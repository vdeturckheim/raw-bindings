#include <napi.h>
#include <clang-c/Index.h>
#include <vector>
#include <memory>
#include <string>

class ClangIndex : public Napi::ObjectWrap<ClangIndex> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::FunctionReference constructor;
    ClangIndex(const Napi::CallbackInfo& info);
    ~ClangIndex();
    
    CXIndex GetIndex() { return index_; }
    
private:
    CXIndex index_;
    
    void Dispose(const Napi::CallbackInfo& info);
};

class ClangTranslationUnit : public Napi::ObjectWrap<ClangTranslationUnit> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::FunctionReference constructor;
    ClangTranslationUnit(const Napi::CallbackInfo& info);
    ~ClangTranslationUnit();
    
    CXTranslationUnit GetTU() { return tu_; }
    CXTranslationUnit tu_;
    
private:
    void Dispose(const Napi::CallbackInfo& info);
    Napi::Value GetCursor(const Napi::CallbackInfo& info);
};

Napi::FunctionReference ClangIndex::constructor;
Napi::FunctionReference ClangTranslationUnit::constructor;

// Helper to convert CXString to Napi::String
Napi::String CXStringToNapi(Napi::Env env, CXString str) {
    const char* cstr = clang_getCString(str);
    Napi::String result = Napi::String::New(env, cstr ? cstr : "");
    clang_disposeString(str);
    return result;
}

// ClangIndex Implementation
ClangIndex::ClangIndex(const Napi::CallbackInfo& info) 
    : Napi::ObjectWrap<ClangIndex>(info) {
    Napi::Env env = info.Env();
    
    int excludeDeclarationsFromPCH = 0;
    int displayDiagnostics = 0;
    
    if (info.Length() >= 1 && info[0].IsBoolean()) {
        excludeDeclarationsFromPCH = info[0].As<Napi::Boolean>().Value() ? 1 : 0;
    }
    if (info.Length() >= 2 && info[1].IsBoolean()) {
        displayDiagnostics = info[1].As<Napi::Boolean>().Value() ? 1 : 0;
    }
    
    index_ = clang_createIndex(excludeDeclarationsFromPCH, displayDiagnostics);
}

ClangIndex::~ClangIndex() {
    if (index_) {
        clang_disposeIndex(index_);
        index_ = nullptr;
    }
}

void ClangIndex::Dispose(const Napi::CallbackInfo& info) {
    if (index_) {
        clang_disposeIndex(index_);
        index_ = nullptr;
    }
}

Napi::Object ClangIndex::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "ClangIndex", {
        InstanceMethod("dispose", &ClangIndex::Dispose),
    });
    
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    
    exports.Set("ClangIndex", func);
    return exports;
}

// ClangTranslationUnit Implementation
ClangTranslationUnit::ClangTranslationUnit(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<ClangTranslationUnit>(info), tu_(nullptr) {
    // TU will be set by parseTranslationUnit function
}

ClangTranslationUnit::~ClangTranslationUnit() {
    if (tu_) {
        clang_disposeTranslationUnit(tu_);
        tu_ = nullptr;
    }
}

void ClangTranslationUnit::Dispose(const Napi::CallbackInfo& info) {
    if (tu_) {
        clang_disposeTranslationUnit(tu_);
        tu_ = nullptr;
    }
}

Napi::Value ClangTranslationUnit::GetCursor(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (!tu_) {
        Napi::TypeError::New(env, "Translation unit is disposed").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXCursor cursor = clang_getTranslationUnitCursor(tu_);
    
    Napi::Object result = Napi::Object::New(env);
    result.Set("_cursor", Napi::External<CXCursor>::New(env, new CXCursor(cursor)));
    result.Set("_tu", Napi::External<CXTranslationUnit>::New(env, &tu_));
    
    return result;
}

Napi::Object ClangTranslationUnit::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "ClangTranslationUnit", {
        InstanceMethod("dispose", &ClangTranslationUnit::Dispose),
        InstanceMethod("getCursor", &ClangTranslationUnit::GetCursor),
    });
    
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    
    exports.Set("ClangTranslationUnit", func);
    return exports;
}

// Free functions
Napi::Value CreateIndex(const Napi::CallbackInfo& info) {
    return ClangIndex::constructor.New({info[0], info[1]});
}

Napi::Value ParseTranslationUnit(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    if (!info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    ClangIndex* index = Napi::ObjectWrap<ClangIndex>::Unwrap(info[0].As<Napi::Object>());
    std::string sourceFilename = info[1].As<Napi::String>().Utf8Value();
    
    // Parse command line arguments if provided
    std::vector<std::string> args;
    std::vector<const char*> argv;
    
    if (info.Length() >= 3 && info[2].IsArray()) {
        Napi::Array arr = info[2].As<Napi::Array>();
        for (uint32_t i = 0; i < arr.Length(); i++) {
            args.push_back(arr.Get(i).As<Napi::String>().Utf8Value());
            argv.push_back(args.back().c_str());
        }
    }
    
    unsigned options = CXTranslationUnit_None;
    if (info.Length() >= 4 && info[3].IsNumber()) {
        options = info[3].As<Napi::Number>().Uint32Value();
    }
    
    CXTranslationUnit tu = clang_parseTranslationUnit(
        index->GetIndex(),
        sourceFilename.c_str(),
        argv.empty() ? nullptr : argv.data(),
        static_cast<int>(argv.size()),
        nullptr,
        0,
        options
    );
    
    if (!tu) {
        Napi::Error::New(env, "Failed to parse translation unit").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    Napi::Object tuObj = ClangTranslationUnit::constructor.New({});
    ClangTranslationUnit* tuWrapper = Napi::ObjectWrap<ClangTranslationUnit>::Unwrap(tuObj);
    tuWrapper->tu_ = tu;
    
    return tuObj;
}

// Cursor functions
Napi::Value GetCursorKind(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (!info[0].IsObject()) {
        Napi::TypeError::New(env, "Expected cursor object").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    Napi::Object cursorObj = info[0].As<Napi::Object>();
    if (!cursorObj.Has("_cursor")) {
        Napi::TypeError::New(env, "Invalid cursor object").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXCursor* cursor = cursorObj.Get("_cursor").As<Napi::External<CXCursor>>().Data();
    return Napi::Number::New(env, clang_getCursorKind(*cursor));
}

Napi::Value GetCursorSpelling(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (!info[0].IsObject()) {
        Napi::TypeError::New(env, "Expected cursor object").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    Napi::Object cursorObj = info[0].As<Napi::Object>();
    CXCursor* cursor = cursorObj.Get("_cursor").As<Napi::External<CXCursor>>().Data();
    
    return CXStringToNapi(env, clang_getCursorSpelling(*cursor));
}

Napi::Value GetCursorLocation(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (!info[0].IsObject()) {
        Napi::TypeError::New(env, "Expected cursor object").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    Napi::Object cursorObj = info[0].As<Napi::Object>();
    CXCursor* cursor = cursorObj.Get("_cursor").As<Napi::External<CXCursor>>().Data();
    
    CXSourceLocation location = clang_getCursorLocation(*cursor);
    
    CXFile file;
    unsigned line, column, offset;
    clang_getFileLocation(location, &file, &line, &column, &offset);
    
    Napi::Object result = Napi::Object::New(env);
    if (file) {
        result.Set("file", CXStringToNapi(env, clang_getFileName(file)));
    } else {
        result.Set("file", env.Null());
    }
    result.Set("line", Napi::Number::New(env, line));
    result.Set("column", Napi::Number::New(env, column));
    result.Set("offset", Napi::Number::New(env, offset));
    
    return result;
}

// Type functions
Napi::Value GetCursorType(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (!info[0].IsObject()) {
        Napi::TypeError::New(env, "Expected cursor object").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    Napi::Object cursorObj = info[0].As<Napi::Object>();
    CXCursor* cursor = cursorObj.Get("_cursor").As<Napi::External<CXCursor>>().Data();
    
    CXType type = clang_getCursorType(*cursor);
    
    Napi::Object result = Napi::Object::New(env);
    result.Set("_type", Napi::External<CXType>::New(env, new CXType(type)));
    
    return result;
}

Napi::Value GetTypeSpelling(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (!info[0].IsObject()) {
        Napi::TypeError::New(env, "Expected type object").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    Napi::Object typeObj = info[0].As<Napi::Object>();
    CXType* type = typeObj.Get("_type").As<Napi::External<CXType>>().Data();
    
    return CXStringToNapi(env, clang_getTypeSpelling(*type));
}

Napi::Value GetTypeNullability(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (!info[0].IsObject()) {
        Napi::TypeError::New(env, "Expected type object").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    Napi::Object typeObj = info[0].As<Napi::Object>();
    CXType* type = typeObj.Get("_type").As<Napi::External<CXType>>().Data();
    
    return Napi::Number::New(env, clang_Type_getNullability(*type));
}

Napi::Value GetResultType(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (!info[0].IsObject()) {
        Napi::TypeError::New(env, "Expected type object").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    Napi::Object typeObj = info[0].As<Napi::Object>();
    CXType* type = typeObj.Get("_type").As<Napi::External<CXType>>().Data();
    
    CXType resultType = clang_getResultType(*type);
    
    Napi::Object result = Napi::Object::New(env);
    result.Set("_type", Napi::External<CXType>::New(env, new CXType(resultType)));
    
    return result;
}

Napi::Value GetNumArgTypes(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (!info[0].IsObject()) {
        Napi::TypeError::New(env, "Expected type object").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    Napi::Object typeObj = info[0].As<Napi::Object>();
    CXType* type = typeObj.Get("_type").As<Napi::External<CXType>>().Data();
    
    return Napi::Number::New(env, clang_getNumArgTypes(*type));
}

Napi::Value GetArgType(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (!info[0].IsObject() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Expected type object and index").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    Napi::Object typeObj = info[0].As<Napi::Object>();
    CXType* type = typeObj.Get("_type").As<Napi::External<CXType>>().Data();
    unsigned index = info[1].As<Napi::Number>().Uint32Value();
    
    CXType argType = clang_getArgType(*type, index);
    
    Napi::Object result = Napi::Object::New(env);
    result.Set("_type", Napi::External<CXType>::New(env, new CXType(argType)));
    
    return result;
}

// Documentation
Napi::Value GetCursorRawCommentText(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (!info[0].IsObject()) {
        Napi::TypeError::New(env, "Expected cursor object").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    Napi::Object cursorObj = info[0].As<Napi::Object>();
    CXCursor* cursor = cursorObj.Get("_cursor").As<Napi::External<CXCursor>>().Data();
    
    CXString comment = clang_Cursor_getRawCommentText(*cursor);
    const char* str = clang_getCString(comment);
    
    if (!str) {
        clang_disposeString(comment);
        return env.Null();
    }
    
    Napi::String result = Napi::String::New(env, str);
    clang_disposeString(comment);
    return result;
}

// Enum constant value
Napi::Value GetEnumConstantDeclValue(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (!info[0].IsObject()) {
        Napi::TypeError::New(env, "Expected cursor object").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    Napi::Object cursorObj = info[0].As<Napi::Object>();
    CXCursor* cursor = cursorObj.Get("_cursor").As<Napi::External<CXCursor>>().Data();
    
    long long value = clang_getEnumConstantDeclValue(*cursor);
    return Napi::Number::New(env, static_cast<double>(value));
}

// Typedef underlying type
Napi::Value GetTypedefDeclUnderlyingType(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (!info[0].IsObject()) {
        Napi::TypeError::New(env, "Expected cursor object").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    Napi::Object cursorObj = info[0].As<Napi::Object>();
    CXCursor* cursor = cursorObj.Get("_cursor").As<Napi::External<CXCursor>>().Data();
    
    CXType underlyingType = clang_getTypedefDeclUnderlyingType(*cursor);
    
    Napi::Object result = Napi::Object::New(env);
    result.Set("_type", Napi::External<CXType>::New(env, new CXType(underlyingType)));
    
    return result;
}

// Get cursor result type (for functions/methods)
Napi::Value GetCursorResultType(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (!info[0].IsObject()) {
        Napi::TypeError::New(env, "Expected cursor object").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    Napi::Object cursorObj = info[0].As<Napi::Object>();
    CXCursor* cursor = cursorObj.Get("_cursor").As<Napi::External<CXCursor>>().Data();
    
    CXType type = clang_getCursorType(*cursor);
    CXType resultType = clang_getResultType(type);
    
    Napi::Object result = Napi::Object::New(env);
    result.Set("_type", Napi::External<CXType>::New(env, new CXType(resultType)));
    
    return result;
}

// Get number of arguments for a cursor
Napi::Value GetNumCursorArguments(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (!info[0].IsObject()) {
        Napi::TypeError::New(env, "Expected cursor object").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    Napi::Object cursorObj = info[0].As<Napi::Object>();
    CXCursor* cursor = cursorObj.Get("_cursor").As<Napi::External<CXCursor>>().Data();
    
    int numArgs = clang_Cursor_getNumArguments(*cursor);
    return Napi::Number::New(env, numArgs);
}

// Get argument cursor at index
Napi::Value GetCursorArgument(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (!info[0].IsObject() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Expected cursor object and index number").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    Napi::Object cursorObj = info[0].As<Napi::Object>();
    CXCursor* cursor = cursorObj.Get("_cursor").As<Napi::External<CXCursor>>().Data();
    unsigned index = info[1].As<Napi::Number>().Uint32Value();
    
    CXCursor argCursor = clang_Cursor_getArgument(*cursor, index);
    
    Napi::Object result = Napi::Object::New(env);
    result.Set("_cursor", Napi::External<CXCursor>::New(env, new CXCursor(argCursor)));
    
    // Store the TU reference if available
    if (cursorObj.Has("_tu")) {
        result.Set("_tu", cursorObj.Get("_tu"));
    }
    
    return result;
}

// Visitor support
struct VisitorData {
    Napi::FunctionReference callback;
    Napi::Env env;
};

enum CXChildVisitResult VisitorCallback(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    VisitorData* data = static_cast<VisitorData*>(client_data);
    
    Napi::HandleScope scope(data->env);
    
    // Create cursor objects
    Napi::Object cursorObj = Napi::Object::New(data->env);
    cursorObj.Set("_cursor", Napi::External<CXCursor>::New(data->env, new CXCursor(cursor)));
    
    Napi::Object parentObj = Napi::Object::New(data->env);
    parentObj.Set("_cursor", Napi::External<CXCursor>::New(data->env, new CXCursor(parent)));
    
    // Call JavaScript callback
    Napi::Value result = data->callback.Call({cursorObj, parentObj});
    
    // Default to continue if callback doesn't return a value
    if (!result.IsNumber()) {
        return CXChildVisit_Continue;
    }
    
    int visitResult = result.As<Napi::Number>().Int32Value();
    return static_cast<CXChildVisitResult>(visitResult);
}

Napi::Value VisitChildren(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsFunction()) {
        Napi::TypeError::New(env, "Expected cursor and callback function").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    Napi::Object cursorObj = info[0].As<Napi::Object>();
    CXCursor* cursor = cursorObj.Get("_cursor").As<Napi::External<CXCursor>>().Data();
    
    VisitorData data = { Napi::Persistent(info[1].As<Napi::Function>()), env };
    
    unsigned result = clang_visitChildren(*cursor, VisitorCallback, &data);
    
    return Napi::Number::New(env, result);
}

// Module initialization
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    // Initialize classes
    ClangIndex::Init(env, exports);
    ClangTranslationUnit::Init(env, exports);
    
    // Export functions
    exports.Set("createIndex", Napi::Function::New(env, CreateIndex));
    exports.Set("parseTranslationUnit", Napi::Function::New(env, ParseTranslationUnit));
    
    // Cursor functions
    exports.Set("getCursorKind", Napi::Function::New(env, GetCursorKind));
    exports.Set("getCursorSpelling", Napi::Function::New(env, GetCursorSpelling));
    exports.Set("getCursorLocation", Napi::Function::New(env, GetCursorLocation));
    exports.Set("getCursorType", Napi::Function::New(env, GetCursorType));
    exports.Set("visitChildren", Napi::Function::New(env, VisitChildren));
    
    // Type functions
    exports.Set("getTypeSpelling", Napi::Function::New(env, GetTypeSpelling));
    exports.Set("getTypeNullability", Napi::Function::New(env, GetTypeNullability));
    exports.Set("getResultType", Napi::Function::New(env, GetResultType));
    exports.Set("getNumArgTypes", Napi::Function::New(env, GetNumArgTypes));
    exports.Set("getArgType", Napi::Function::New(env, GetArgType));
    
    // Documentation
    exports.Set("getCursorRawCommentText", Napi::Function::New(env, GetCursorRawCommentText));
    
    // Enum functions
    exports.Set("getEnumConstantDeclValue", Napi::Function::New(env, GetEnumConstantDeclValue));
    
    // Typedef functions
    exports.Set("getTypedefDeclUnderlyingType", Napi::Function::New(env, GetTypedefDeclUnderlyingType));
    
    // Cursor argument functions
    exports.Set("getCursorResultType", Napi::Function::New(env, GetCursorResultType));
    exports.Set("getNumCursorArguments", Napi::Function::New(env, GetNumCursorArguments));
    exports.Set("getCursorArgument", Napi::Function::New(env, GetCursorArgument));
    
    // Export CXChildVisitResult enum values
    exports.Set("CXChildVisit_Break", Napi::Number::New(env, CXChildVisit_Break));
    exports.Set("CXChildVisit_Continue", Napi::Number::New(env, CXChildVisit_Continue));
    exports.Set("CXChildVisit_Recurse", Napi::Number::New(env, CXChildVisit_Recurse));
    
    // Export common CXCursorKind values
    exports.Set("CXCursor_UnexposedDecl", Napi::Number::New(env, CXCursor_UnexposedDecl));
    exports.Set("CXCursor_StructDecl", Napi::Number::New(env, CXCursor_StructDecl));
    exports.Set("CXCursor_UnionDecl", Napi::Number::New(env, CXCursor_UnionDecl));
    exports.Set("CXCursor_ClassDecl", Napi::Number::New(env, CXCursor_ClassDecl));
    exports.Set("CXCursor_EnumDecl", Napi::Number::New(env, CXCursor_EnumDecl));
    exports.Set("CXCursor_FieldDecl", Napi::Number::New(env, CXCursor_FieldDecl));
    exports.Set("CXCursor_FunctionDecl", Napi::Number::New(env, CXCursor_FunctionDecl));
    exports.Set("CXCursor_VarDecl", Napi::Number::New(env, CXCursor_VarDecl));
    exports.Set("CXCursor_ParmDecl", Napi::Number::New(env, CXCursor_ParmDecl));
    exports.Set("CXCursor_TypedefDecl", Napi::Number::New(env, CXCursor_TypedefDecl));
    exports.Set("CXCursor_CXXMethod", Napi::Number::New(env, CXCursor_CXXMethod));
    exports.Set("CXCursor_EnumConstantDecl", Napi::Number::New(env, CXCursor_EnumConstantDecl));
    
    // Objective-C cursor kinds
    exports.Set("CXCursor_ObjCInterfaceDecl", Napi::Number::New(env, CXCursor_ObjCInterfaceDecl));
    exports.Set("CXCursor_ObjCCategoryDecl", Napi::Number::New(env, CXCursor_ObjCCategoryDecl));
    exports.Set("CXCursor_ObjCProtocolDecl", Napi::Number::New(env, CXCursor_ObjCProtocolDecl));
    exports.Set("CXCursor_ObjCPropertyDecl", Napi::Number::New(env, CXCursor_ObjCPropertyDecl));
    exports.Set("CXCursor_ObjCInstanceMethodDecl", Napi::Number::New(env, CXCursor_ObjCInstanceMethodDecl));
    exports.Set("CXCursor_ObjCClassMethodDecl", Napi::Number::New(env, CXCursor_ObjCClassMethodDecl));
    exports.Set("CXCursor_ObjCProtocolRef", Napi::Number::New(env, CXCursor_ObjCProtocolRef));
    
    // Parse options
    exports.Set("CXTranslationUnit_None", Napi::Number::New(env, CXTranslationUnit_None));
    exports.Set("CXTranslationUnit_DetailedPreprocessingRecord", Napi::Number::New(env, CXTranslationUnit_DetailedPreprocessingRecord));
    exports.Set("CXTranslationUnit_IncludeBriefCommentsInCodeCompletion", Napi::Number::New(env, CXTranslationUnit_IncludeBriefCommentsInCodeCompletion));
    
    return exports;
}

NODE_API_MODULE(node_clang, Init)