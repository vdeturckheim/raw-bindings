#include <napi.h>
#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include <clang-c/Index.h>

// Helper function to wrap pointers as JavaScript objects
static Napi::Object wrapPointer(Napi::Env env, void* ptr, const std::string& typeName) {
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("_ptr", Napi::External<void>::New(env, ptr));
    obj.Set("_type", Napi::String::New(env, typeName));
    return obj;
}

// Overload for const pointers
static Napi::Object wrapPointer(Napi::Env env, const void* ptr, const std::string& typeName) {
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("_ptr", Napi::External<void>::New(env, const_cast<void*>(ptr)));
    obj.Set("_type", Napi::String::New(env, typeName));
    obj.Set("_const", Napi::Boolean::New(env, true));
    return obj;
}

// Helper function to wrap const pointers as JavaScript objects (kept for compatibility)
static Napi::Object wrapConstPointer(Napi::Env env, const void* ptr, const std::string& typeName) {
    return wrapPointer(env, ptr, typeName);
}

// Helper to unwrap pointer from JavaScript object
static void* unwrapPointer(Napi::Object obj) {
    if (!obj.Has("_ptr")) {
        return nullptr;
    }
    return obj.Get("_ptr").As<Napi::External<void>>().Data();
}

// Helper to wrap owned pointers with a finalizer
template <typename T>
static Napi::Object wrapOwnedPointer(Napi::Env env, T* ptr, const std::string& typeName) {
    auto ext = Napi::External<T>::New(env, ptr, [](Napi::Env, T* p) {
        delete p;
    });
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("_ptr", ext);
    obj.Set("_type", Napi::String::New(env, typeName));
    return obj;
}

// Enum constants
// Represents a specific kind of binary operator which can appear at a cursor.
// enum CX_BinaryOperatorKind
static Napi::Value Get_CX_BO_Add(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_Add);
}

static Napi::Value Get_CX_BO_AddAssign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_AddAssign);
}

static Napi::Value Get_CX_BO_And(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_And);
}

static Napi::Value Get_CX_BO_AndAssign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_AndAssign);
}

static Napi::Value Get_CX_BO_Assign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_Assign);
}

static Napi::Value Get_CX_BO_Cmp(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_Cmp);
}

static Napi::Value Get_CX_BO_Comma(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_Comma);
}

static Napi::Value Get_CX_BO_Div(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_Div);
}

static Napi::Value Get_CX_BO_DivAssign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_DivAssign);
}

static Napi::Value Get_CX_BO_EQ(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_EQ);
}

static Napi::Value Get_CX_BO_GE(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_GE);
}

static Napi::Value Get_CX_BO_GT(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_GT);
}

static Napi::Value Get_CX_BO_Invalid(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_Invalid);
}

static Napi::Value Get_CX_BO_LAnd(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_LAnd);
}

static Napi::Value Get_CX_BO_LAST(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_LAST);
}

static Napi::Value Get_CX_BO_LE(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_LE);
}

static Napi::Value Get_CX_BO_LOr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_LOr);
}

static Napi::Value Get_CX_BO_LT(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_LT);
}

static Napi::Value Get_CX_BO_Mul(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_Mul);
}

static Napi::Value Get_CX_BO_MulAssign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_MulAssign);
}

static Napi::Value Get_CX_BO_NE(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_NE);
}

static Napi::Value Get_CX_BO_Or(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_Or);
}

static Napi::Value Get_CX_BO_OrAssign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_OrAssign);
}

static Napi::Value Get_CX_BO_PtrMemD(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_PtrMemD);
}

static Napi::Value Get_CX_BO_PtrMemI(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_PtrMemI);
}

static Napi::Value Get_CX_BO_Rem(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_Rem);
}

static Napi::Value Get_CX_BO_RemAssign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_RemAssign);
}

static Napi::Value Get_CX_BO_Shl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_Shl);
}

static Napi::Value Get_CX_BO_ShlAssign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_ShlAssign);
}

static Napi::Value Get_CX_BO_Shr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_Shr);
}

static Napi::Value Get_CX_BO_ShrAssign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_ShrAssign);
}

static Napi::Value Get_CX_BO_Sub(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_Sub);
}

static Napi::Value Get_CX_BO_SubAssign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_SubAssign);
}

static Napi::Value Get_CX_BO_Xor(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_Xor);
}

static Napi::Value Get_CX_BO_XorAssign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_BO_XorAssign);
}

// Represents the C++ access control level to a base class for a
// cursor with kind CX_CXXBaseSpecifier.
// enum CX_CXXAccessSpecifier
static Napi::Value Get_CX_CXXInvalidAccessSpecifier(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_CXXInvalidAccessSpecifier);
}

static Napi::Value Get_CX_CXXPrivate(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_CXXPrivate);
}

static Napi::Value Get_CX_CXXProtected(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_CXXProtected);
}

static Napi::Value Get_CX_CXXPublic(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_CXXPublic);
}

// Represents the storage classes as declared in the source. CX_SC_Invalid
// was added for the case that the passed cursor in not a declaration.
// enum CX_StorageClass
static Napi::Value Get_CX_SC_Auto(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_SC_Auto);
}

static Napi::Value Get_CX_SC_Extern(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_SC_Extern);
}

static Napi::Value Get_CX_SC_Invalid(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_SC_Invalid);
}

static Napi::Value Get_CX_SC_None(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_SC_None);
}

static Napi::Value Get_CX_SC_OpenCLWorkGroupLocal(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_SC_OpenCLWorkGroupLocal);
}

static Napi::Value Get_CX_SC_PrivateExtern(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_SC_PrivateExtern);
}

static Napi::Value Get_CX_SC_Register(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_SC_Register);
}

static Napi::Value Get_CX_SC_Static(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CX_SC_Static);
}

// Describes the availability of a particular entity, which indicates
// whether the use of this entity will result in a warning or error due to
// it being deprecated or unavailable.
// enum CXAvailabilityKind
// The entity is available.
static Napi::Value Get_CXAvailability_Available(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXAvailability_Available);
}

// The entity is available, but has been deprecated (and its use is
// not recommended).
static Napi::Value Get_CXAvailability_Deprecated(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXAvailability_Deprecated);
}

// The entity is available, but not accessible; any use of it will be
// an error.
static Napi::Value Get_CXAvailability_NotAccessible(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXAvailability_NotAccessible);
}

// The entity is not available; any use of it will be an error.
static Napi::Value Get_CXAvailability_NotAvailable(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXAvailability_NotAvailable);
}

// Describes the kind of binary operators.
// enum CXBinaryOperatorKind
// Addition operator.
static Napi::Value Get_CXBinaryOperator_Add(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_Add);
}

// Addition assignment operator.
static Napi::Value Get_CXBinaryOperator_AddAssign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_AddAssign);
}

// Bitwise AND operator.
static Napi::Value Get_CXBinaryOperator_And(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_And);
}

// Bitwise AND assignment operator.
static Napi::Value Get_CXBinaryOperator_AndAssign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_AndAssign);
}

// Assignment operator.
static Napi::Value Get_CXBinaryOperator_Assign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_Assign);
}

// C++ three-way comparison (spaceship) operator.
static Napi::Value Get_CXBinaryOperator_Cmp(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_Cmp);
}

// Comma operator.
static Napi::Value Get_CXBinaryOperator_Comma(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_Comma);
}

// Division operator.
static Napi::Value Get_CXBinaryOperator_Div(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_Div);
}

// Division assignment operator.
static Napi::Value Get_CXBinaryOperator_DivAssign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_DivAssign);
}

// Equal operator.
static Napi::Value Get_CXBinaryOperator_EQ(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_EQ);
}

// Greater or equal operator.
static Napi::Value Get_CXBinaryOperator_GE(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_GE);
}

// Greater than operator.
static Napi::Value Get_CXBinaryOperator_GT(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_GT);
}

// This value describes cursors which are not binary operators.
static Napi::Value Get_CXBinaryOperator_Invalid(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_Invalid);
}

// Logical AND operator.
static Napi::Value Get_CXBinaryOperator_LAnd(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_LAnd);
}

// Less or equal operator.
static Napi::Value Get_CXBinaryOperator_LE(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_LE);
}

// Logical OR operator.
static Napi::Value Get_CXBinaryOperator_LOr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_LOr);
}

// Less than operator.
static Napi::Value Get_CXBinaryOperator_LT(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_LT);
}

// Multiplication operator.
static Napi::Value Get_CXBinaryOperator_Mul(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_Mul);
}

// Multiplication assignment operator.
static Napi::Value Get_CXBinaryOperator_MulAssign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_MulAssign);
}

// Not equal operator.
static Napi::Value Get_CXBinaryOperator_NE(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_NE);
}

// Bitwise OR operator.
static Napi::Value Get_CXBinaryOperator_Or(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_Or);
}

// Bitwise OR assignment operator.
static Napi::Value Get_CXBinaryOperator_OrAssign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_OrAssign);
}

// C++ Pointer - to - member operator.
static Napi::Value Get_CXBinaryOperator_PtrMemD(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_PtrMemD);
}

// C++ Pointer - to - member operator.
static Napi::Value Get_CXBinaryOperator_PtrMemI(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_PtrMemI);
}

// Remainder operator.
static Napi::Value Get_CXBinaryOperator_Rem(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_Rem);
}

// Remainder assignment operator.
static Napi::Value Get_CXBinaryOperator_RemAssign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_RemAssign);
}

// Bitwise shift left operator.
static Napi::Value Get_CXBinaryOperator_Shl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_Shl);
}

// Bitwise shift left assignment operator.
static Napi::Value Get_CXBinaryOperator_ShlAssign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_ShlAssign);
}

// Bitwise shift right operator.
static Napi::Value Get_CXBinaryOperator_Shr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_Shr);
}

// Bitwise shift right assignment operator.
static Napi::Value Get_CXBinaryOperator_ShrAssign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_ShrAssign);
}

// Subtraction operator.
static Napi::Value Get_CXBinaryOperator_Sub(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_Sub);
}

// Subtraction assignment operator.
static Napi::Value Get_CXBinaryOperator_SubAssign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_SubAssign);
}

// Bitwise XOR operator.
static Napi::Value Get_CXBinaryOperator_Xor(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_Xor);
}

// Bitwise XOR assignment operator.
static Napi::Value Get_CXBinaryOperator_XorAssign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXBinaryOperator_XorAssign);
}

// Describes the calling convention of a function type
// enum CXCallingConv
static Napi::Value Get_CXCallingConv_AAPCS(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_AAPCS);
}

static Napi::Value Get_CXCallingConv_AAPCS_VFP(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_AAPCS_VFP);
}

static Napi::Value Get_CXCallingConv_AArch64SVEPCS(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_AArch64SVEPCS);
}

static Napi::Value Get_CXCallingConv_AArch64VectorCall(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_AArch64VectorCall);
}

static Napi::Value Get_CXCallingConv_C(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_C);
}

static Napi::Value Get_CXCallingConv_Default(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_Default);
}

static Napi::Value Get_CXCallingConv_IntelOclBicc(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_IntelOclBicc);
}

static Napi::Value Get_CXCallingConv_Invalid(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_Invalid);
}

static Napi::Value Get_CXCallingConv_M68kRTD(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_M68kRTD);
}

static Napi::Value Get_CXCallingConv_PreserveAll(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_PreserveAll);
}

static Napi::Value Get_CXCallingConv_PreserveMost(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_PreserveMost);
}

static Napi::Value Get_CXCallingConv_PreserveNone(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_PreserveNone);
}

static Napi::Value Get_CXCallingConv_RISCVVectorCall(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_RISCVVectorCall);
}

static Napi::Value Get_CXCallingConv_Swift(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_Swift);
}

static Napi::Value Get_CXCallingConv_SwiftAsync(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_SwiftAsync);
}

static Napi::Value Get_CXCallingConv_Unexposed(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_Unexposed);
}

static Napi::Value Get_CXCallingConv_Win64(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_Win64);
}

static Napi::Value Get_CXCallingConv_X86_64SysV(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_X86_64SysV);
}

static Napi::Value Get_CXCallingConv_X86_64Win64(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_X86_64Win64);
}

static Napi::Value Get_CXCallingConv_X86FastCall(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_X86FastCall);
}

static Napi::Value Get_CXCallingConv_X86Pascal(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_X86Pascal);
}

static Napi::Value Get_CXCallingConv_X86RegCall(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_X86RegCall);
}

static Napi::Value Get_CXCallingConv_X86StdCall(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_X86StdCall);
}

static Napi::Value Get_CXCallingConv_X86ThisCall(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_X86ThisCall);
}

static Napi::Value Get_CXCallingConv_X86VectorCall(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCallingConv_X86VectorCall);
}

// Describes how the traversal of the children of a particular
// cursor should proceed after visiting a particular child cursor.
// 
// A value of this enumeration type should be returned by each
// \c CXCursorVisitor to indicate how clang_visitChildren() proceed.
// enum CXChildVisitResult
// Terminates the cursor traversal.
static Napi::Value Get_CXChildVisit_Break(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXChildVisit_Break);
}

// Continues the cursor traversal with the next sibling of
// the cursor just visited, without visiting its children.
static Napi::Value Get_CXChildVisit_Continue(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXChildVisit_Continue);
}

// Recursively traverse the children of this cursor, using
// the same visitor and client data.
static Napi::Value Get_CXChildVisit_Recurse(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXChildVisit_Recurse);
}

// enum CXChoice
// Use the default value of an option that may depend on the process
// environment.
static Napi::Value Get_CXChoice_Default(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXChoice_Default);
}

// Disable the option.
static Napi::Value Get_CXChoice_Disabled(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXChoice_Disabled);
}

// Enable the option.
static Napi::Value Get_CXChoice_Enabled(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXChoice_Enabled);
}

// enum CXChoice
// Flags that can be passed to \c clang_codeCompleteAt() to
// modify its behavior.
// 
// The enumerators in this enumeration can be bitwise-OR'd together to
// provide multiple options to \c clang_codeCompleteAt().
// enum CXCodeComplete_Flags
// Whether to include brief documentation within the set of code
// completions returned.
static Napi::Value Get_CXCodeComplete_IncludeBriefComments(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCodeComplete_IncludeBriefComments);
}

// Whether to include code patterns for language constructs
// within the set of code completions, e.g., for loops.
static Napi::Value Get_CXCodeComplete_IncludeCodePatterns(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCodeComplete_IncludeCodePatterns);
}

// Whether to include completions with small
// fix-its, e.g. change '.' to '->' on member access, etc.
static Napi::Value Get_CXCodeComplete_IncludeCompletionsWithFixIts(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCodeComplete_IncludeCompletionsWithFixIts);
}

// Whether to include macros within the set of code
// completions returned.
static Napi::Value Get_CXCodeComplete_IncludeMacros(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCodeComplete_IncludeMacros);
}

// Whether to speed up completion by omitting top- or namespace-level entities
// defined in the preamble. There's no guarantee any particular entity is
// omitted. This may be useful if the headers are indexed externally.
static Napi::Value Get_CXCodeComplete_SkipPreamble(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCodeComplete_SkipPreamble);
}

// Describes a single piece of text within a code-completion string.
// 
// Each "chunk" within a code-completion string (\c CXCompletionString) is
// either a piece of text with a specific "kind" that describes how that text
// should be interpreted by the client or is another completion string.
// enum CXCompletionChunkKind
// A colon (':').
static Napi::Value Get_CXCompletionChunk_Colon(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_Colon);
}

// A comma separator (',').
static Napi::Value Get_CXCompletionChunk_Comma(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_Comma);
}

// Text that describes the current parameter when code-completion is
// referring to function call, message send, or template specialization.
// 
// A "current parameter" chunk occurs when code-completion is providing
// information about a parameter corresponding to the argument at the
// code-completion point. For example, given a function
// 
// \code
// int add(int x, int y);
// \endcode
// 
// and the source code \c add(, where the code-completion point is after the
// "(", the code-completion string will contain a "current parameter" chunk
// for "int x", indicating that the current argument will initialize that
// parameter. After typing further, to \c add(17, (where the code-completion
// point is after the ","), the code-completion string will contain a
// "current parameter" chunk to "int y".
static Napi::Value Get_CXCompletionChunk_CurrentParameter(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_CurrentParameter);
}

// An '=' sign.
static Napi::Value Get_CXCompletionChunk_Equal(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_Equal);
}

// Horizontal space (' ').
static Napi::Value Get_CXCompletionChunk_HorizontalSpace(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_HorizontalSpace);
}

// Informative text that should be displayed but never inserted as
// part of the template.
// 
// An "informative" chunk contains annotations that can be displayed to
// help the user decide whether a particular code-completion result is the
// right option, but which is not part of the actual template to be inserted
// by code completion.
static Napi::Value Get_CXCompletionChunk_Informative(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_Informative);
}

// A left angle bracket ('<').
static Napi::Value Get_CXCompletionChunk_LeftAngle(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_LeftAngle);
}

// A left brace ('{').
static Napi::Value Get_CXCompletionChunk_LeftBrace(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_LeftBrace);
}

// A left bracket ('[').
static Napi::Value Get_CXCompletionChunk_LeftBracket(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_LeftBracket);
}

// A left parenthesis ('('), used to initiate a function call or
// signal the beginning of a function parameter list.
static Napi::Value Get_CXCompletionChunk_LeftParen(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_LeftParen);
}

// A code-completion string that describes "optional" text that
// could be a part of the template (but is not required).
// 
// The Optional chunk is the only kind of chunk that has a code-completion
// string for its representation, which is accessible via
// \c clang_getCompletionChunkCompletionString(). The code-completion string
// describes an additional part of the template that is completely optional.
// For example, optional chunks can be used to describe the placeholders for
// arguments that match up with defaulted function parameters, e.g. given:
// 
// \code
// void f(int x, float y = 3.14, double z = 2.71828);
// \endcode
// 
// The code-completion string for this function would contain:
// - a TypedText chunk for "f".
// - a LeftParen chunk for "(".
// - a Placeholder chunk for "int x"
// - an Optional chunk containing the remaining defaulted arguments, e.g.,
// - a Comma chunk for ","
// - a Placeholder chunk for "float y"
// - an Optional chunk containing the last defaulted argument:
// - a Comma chunk for ","
// - a Placeholder chunk for "double z"
// - a RightParen chunk for ")"
// 
// There are many ways to handle Optional chunks. Two simple approaches are:
// - Completely ignore optional chunks, in which case the template for the
// function "f" would only include the first parameter ("int x").
// - Fully expand all optional chunks, in which case the template for the
// function "f" would have all of the parameters.
static Napi::Value Get_CXCompletionChunk_Optional(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_Optional);
}

// Placeholder text that should be replaced by the user.
// 
// A "placeholder" chunk marks a place where the user should insert text
// into the code-completion template. For example, placeholders might mark
// the function parameters for a function declaration, to indicate that the
// user should provide arguments for each of those parameters. The actual
// text in a placeholder is a suggestion for the text to display before
// the user replaces the placeholder with real code.
static Napi::Value Get_CXCompletionChunk_Placeholder(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_Placeholder);
}

// Text that specifies the result type of a given result.
// 
// This special kind of informative chunk is not meant to be inserted into
// the text buffer. Rather, it is meant to illustrate the type that an
// expression using the given completion string would have.
static Napi::Value Get_CXCompletionChunk_ResultType(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_ResultType);
}

// A right angle bracket ('>').
static Napi::Value Get_CXCompletionChunk_RightAngle(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_RightAngle);
}

// A right brace ('}').
static Napi::Value Get_CXCompletionChunk_RightBrace(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_RightBrace);
}

// A right bracket (']').
static Napi::Value Get_CXCompletionChunk_RightBracket(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_RightBracket);
}

// A right parenthesis (')'), used to finish a function call or
// signal the end of a function parameter list.
static Napi::Value Get_CXCompletionChunk_RightParen(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_RightParen);
}

// A semicolon (';').
static Napi::Value Get_CXCompletionChunk_SemiColon(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_SemiColon);
}

// Text that should be inserted as part of a code-completion result.
// 
// A "text" chunk represents text that is part of the template to be
// inserted into user code should this particular code-completion result
// be selected.
static Napi::Value Get_CXCompletionChunk_Text(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_Text);
}

// Text that a user would be expected to type to get this
// code-completion result.
// 
// There will be exactly one "typed text" chunk in a semantic string, which
// will typically provide the spelling of a keyword or the name of a
// declaration that could be used at the current code point. Clients are
// expected to filter the code-completion results based on the text in this
// chunk.
static Napi::Value Get_CXCompletionChunk_TypedText(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_TypedText);
}

// Vertical space ('\\n'), after which it is generally a good idea to
// perform indentation.
static Napi::Value Get_CXCompletionChunk_VerticalSpace(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionChunk_VerticalSpace);
}

// Bits that represent the context under which completion is occurring.
// 
// The enumerators in this enumeration may be bitwise-OR'd together if multiple
// contexts are occurring simultaneously.
// enum CXCompletionContext
// Completions for any possible type should be included in the results.
static Napi::Value Get_CXCompletionContext_AnyType(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_AnyType);
}

// Completions for any possible value (variables, function calls, etc.)
// should be included in the results.
static Napi::Value Get_CXCompletionContext_AnyValue(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_AnyValue);
}

// Completions for fields of the member being accessed using the arrow
// operator should be included in the results.
static Napi::Value Get_CXCompletionContext_ArrowMemberAccess(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_ArrowMemberAccess);
}

// Completions for C++ class names should be included in the results.
static Napi::Value Get_CXCompletionContext_ClassTag(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_ClassTag);
}

// Completions for values that resolve to a C++ class type should be
// included in the results.
static Napi::Value Get_CXCompletionContext_CXXClassTypeValue(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_CXXClassTypeValue);
}

// Completions for fields of the member being accessed using the dot
// operator should be included in the results.
static Napi::Value Get_CXCompletionContext_DotMemberAccess(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_DotMemberAccess);
}

// Completions for enum tags should be included in the results.
static Napi::Value Get_CXCompletionContext_EnumTag(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_EnumTag);
}

// #include file completions should be included in the results.
static Napi::Value Get_CXCompletionContext_IncludedFile(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_IncludedFile);
}

// Completions for preprocessor macro names should be included in
// the results.
static Napi::Value Get_CXCompletionContext_MacroName(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_MacroName);
}

// Completions for C++ namespaces and namespace aliases should be
// included in the results.
static Napi::Value Get_CXCompletionContext_Namespace(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_Namespace);
}

// Natural language completions should be included in the results.
static Napi::Value Get_CXCompletionContext_NaturalLanguage(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_NaturalLanguage);
}

// Completions for C++ nested name specifiers should be included in
// the results.
static Napi::Value Get_CXCompletionContext_NestedNameSpecifier(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_NestedNameSpecifier);
}

// Completions for Objective-C categories should be included in
// the results.
static Napi::Value Get_CXCompletionContext_ObjCCategory(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_ObjCCategory);
}

// Completions for Objective-C class messages should be included in
// the results.
static Napi::Value Get_CXCompletionContext_ObjCClassMessage(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_ObjCClassMessage);
}

// Completions for Objective-C instance messages should be included
// in the results.
static Napi::Value Get_CXCompletionContext_ObjCInstanceMessage(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_ObjCInstanceMessage);
}

// Completions for Objective-C interfaces (classes) should be included
// in the results.
static Napi::Value Get_CXCompletionContext_ObjCInterface(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_ObjCInterface);
}

// Completions for values that resolve to an Objective-C object should
// be included in the results.
static Napi::Value Get_CXCompletionContext_ObjCObjectValue(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_ObjCObjectValue);
}

// Completions for properties of the Objective-C object being accessed
// using the dot operator should be included in the results.
static Napi::Value Get_CXCompletionContext_ObjCPropertyAccess(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_ObjCPropertyAccess);
}

// Completions for Objective-C protocols should be included in
// the results.
static Napi::Value Get_CXCompletionContext_ObjCProtocol(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_ObjCProtocol);
}

// Completions for Objective-C selector names should be included in
// the results.
static Napi::Value Get_CXCompletionContext_ObjCSelectorName(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_ObjCSelectorName);
}

// Completions for values that resolve to an Objective-C selector
// should be included in the results.
static Napi::Value Get_CXCompletionContext_ObjCSelectorValue(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_ObjCSelectorValue);
}

// Completions for struct tags should be included in the results.
static Napi::Value Get_CXCompletionContext_StructTag(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_StructTag);
}

// The context for completions is unexposed, as only Clang results
// should be included. (This is equivalent to having no context bits set.)
static Napi::Value Get_CXCompletionContext_Unexposed(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_Unexposed);
}

// Completions for union tags should be included in the results.
static Napi::Value Get_CXCompletionContext_UnionTag(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_UnionTag);
}

// The current context is unknown, so set all contexts.
static Napi::Value Get_CXCompletionContext_Unknown(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCompletionContext_Unknown);
}

// Describes the exception specification of a cursor.
// 
// A negative value indicates that the cursor is not a function declaration.
// enum CXCursor_ExceptionSpecificationKind
// The cursor has exception specification basic noexcept.
static Napi::Value Get_CXCursor_ExceptionSpecificationKind_BasicNoexcept(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ExceptionSpecificationKind_BasicNoexcept);
}

// The cursor has exception specification computed noexcept.
static Napi::Value Get_CXCursor_ExceptionSpecificationKind_ComputedNoexcept(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ExceptionSpecificationKind_ComputedNoexcept);
}

// The cursor has exception specification throw(T1, T2)
static Napi::Value Get_CXCursor_ExceptionSpecificationKind_Dynamic(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ExceptionSpecificationKind_Dynamic);
}

// The cursor has exception specification throw()
static Napi::Value Get_CXCursor_ExceptionSpecificationKind_DynamicNone(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ExceptionSpecificationKind_DynamicNone);
}

// The cursor has exception specification throw(...).
static Napi::Value Get_CXCursor_ExceptionSpecificationKind_MSAny(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ExceptionSpecificationKind_MSAny);
}

// The cursor has no exception specification.
static Napi::Value Get_CXCursor_ExceptionSpecificationKind_None(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ExceptionSpecificationKind_None);
}

// The cursor has a __declspec(nothrow) exception specification.
static Napi::Value Get_CXCursor_ExceptionSpecificationKind_NoThrow(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ExceptionSpecificationKind_NoThrow);
}

// The exception specification has not yet been evaluated.
static Napi::Value Get_CXCursor_ExceptionSpecificationKind_Unevaluated(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ExceptionSpecificationKind_Unevaluated);
}

// The exception specification has not yet been instantiated.
static Napi::Value Get_CXCursor_ExceptionSpecificationKind_Uninstantiated(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ExceptionSpecificationKind_Uninstantiated);
}

// The exception specification has not been parsed yet.
static Napi::Value Get_CXCursor_ExceptionSpecificationKind_Unparsed(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ExceptionSpecificationKind_Unparsed);
}

// Describes the kind of entity that a cursor refers to.
// enum CXCursorKind
// The GNU address of label extension, representing &&label.
static Napi::Value Get_CXCursor_AddrLabelExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_AddrLabelExpr);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_AlignedAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_AlignedAttr);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_AnnotateAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_AnnotateAttr);
}

// OpenMP 5.0 [2.1.5, Array Section].
// OpenACC 3.3 [2.7.1, Data Specification for Data Clauses (Sub Arrays)]
static Napi::Value Get_CXCursor_ArraySectionExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ArraySectionExpr);
}

// [C99 6.5.2.1] Array Subscripting.
static Napi::Value Get_CXCursor_ArraySubscriptExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ArraySubscriptExpr);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_AsmLabelAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_AsmLabelAttr);
}

// A GCC inline assembly statement extension.
static Napi::Value Get_CXCursor_AsmStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_AsmStmt);
}

// A builtin binary operation expression such as "x + y" or
// "x <= y".
static Napi::Value Get_CXCursor_BinaryOperator(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_BinaryOperator);
}

// An expression that represents a block literal.
static Napi::Value Get_CXCursor_BlockExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_BlockExpr);
}

// A break statement.
static Napi::Value Get_CXCursor_BreakStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_BreakStmt);
}

// C++2a std::bit_cast expression.
static Napi::Value Get_CXCursor_BuiltinBitCastExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_BuiltinBitCastExpr);
}

// An expression that calls a function.
static Napi::Value Get_CXCursor_CallExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CallExpr);
}

// A case statement.
static Napi::Value Get_CXCursor_CaseStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CaseStmt);
}

// A character literal.
static Napi::Value Get_CXCursor_CharacterLiteral(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CharacterLiteral);
}

// A C++ class.
static Napi::Value Get_CXCursor_ClassDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ClassDecl);
}

// A C++ class template.
static Napi::Value Get_CXCursor_ClassTemplate(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ClassTemplate);
}

// A C++ class template partial specialization.
static Napi::Value Get_CXCursor_ClassTemplatePartialSpecialization(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ClassTemplatePartialSpecialization);
}

// Compound assignment such as "+=".
static Napi::Value Get_CXCursor_CompoundAssignOperator(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CompoundAssignOperator);
}

// [C99 6.5.2.5]
static Napi::Value Get_CXCursor_CompoundLiteralExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CompoundLiteralExpr);
}

// A group of statements like { stmt stmt }.
// 
// This cursor kind is used to describe compound statements, e.g. function
// bodies.
static Napi::Value Get_CXCursor_CompoundStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CompoundStmt);
}

// a concept declaration.
static Napi::Value Get_CXCursor_ConceptDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ConceptDecl);
}

// Expression that references a C++20 concept.
static Napi::Value Get_CXCursor_ConceptSpecializationExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ConceptSpecializationExpr);
}

// The ?: ternary operator.
static Napi::Value Get_CXCursor_ConditionalOperator(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ConditionalOperator);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_ConstAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ConstAttr);
}

// A C++ constructor.
static Napi::Value Get_CXCursor_Constructor(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_Constructor);
}

// A continue statement.
static Napi::Value Get_CXCursor_ContinueStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ContinueStmt);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_ConvergentAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ConvergentAttr);
}

// A C++ conversion function.
static Napi::Value Get_CXCursor_ConversionFunction(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ConversionFunction);
}

// An explicit cast in C (C99 6.5.4) or a C-style cast in C++
// (C++ [expr.cast]), which uses the syntax (Type)expr.
// 
// For example: (int)f.
static Napi::Value Get_CXCursor_CStyleCastExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CStyleCastExpr);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_CUDAConstantAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CUDAConstantAttr);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_CUDADeviceAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CUDADeviceAttr);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_CUDAGlobalAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CUDAGlobalAttr);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_CUDAHostAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CUDAHostAttr);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_CUDASharedAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CUDASharedAttr);
}

// An access specifier.
static Napi::Value Get_CXCursor_CXXAccessSpecifier(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXAccessSpecifier);
}

// OpenCL's addrspace_cast<> expression.
static Napi::Value Get_CXCursor_CXXAddrspaceCastExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXAddrspaceCastExpr);
}

// A reference to a type declaration.
// 
// A type reference occurs anywhere where a type is named but not
// declared. For example, given:
// 
// \code
// typedef unsigned size_type;
// size_type size;
// \endcode
// 
// The typedef is a declaration of size_type (CXCursor_TypedefDecl),
// while the type of the variable "size" is referenced. The cursor
// referenced by the type of size is the typedef for size_type.
static Napi::Value Get_CXCursor_CXXBaseSpecifier(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXBaseSpecifier);
}

// [C++ 2.13.5] C++ Boolean Literal.
static Napi::Value Get_CXCursor_CXXBoolLiteralExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXBoolLiteralExpr);
}

// C++'s catch statement.
static Napi::Value Get_CXCursor_CXXCatchStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXCatchStmt);
}

// C++'s const_cast<> expression.
static Napi::Value Get_CXCursor_CXXConstCastExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXConstCastExpr);
}

// A delete expression for memory deallocation and destructor calls,
// e.g. "delete[] pArray".
static Napi::Value Get_CXCursor_CXXDeleteExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXDeleteExpr);
}

// C++'s dynamic_cast<> expression.
static Napi::Value Get_CXCursor_CXXDynamicCastExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXDynamicCastExpr);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_CXXFinalAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXFinalAttr);
}

// C++'s for (* : *) statement.
static Napi::Value Get_CXCursor_CXXForRangeStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXForRangeStmt);
}

// Represents an explicit C++ type conversion that uses "functional"
// notion (C++ [expr.type.conv]).
// 
// Example:
// \code
// x = int(0.5);
// \endcode
static Napi::Value Get_CXCursor_CXXFunctionalCastExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXFunctionalCastExpr);
}

// A C++ class method.
static Napi::Value Get_CXCursor_CXXMethod(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXMethod);
}

// A new expression for memory allocation and constructor calls, e.g:
// "new CXXNewExpr(foo)".
static Napi::Value Get_CXCursor_CXXNewExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXNewExpr);
}

// [C++0x 2.14.7] C++ Pointer Literal.
static Napi::Value Get_CXCursor_CXXNullPtrLiteralExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXNullPtrLiteralExpr);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_CXXOverrideAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXOverrideAttr);
}

// Expression that references a C++20 parenthesized list aggregate
// initializer.
static Napi::Value Get_CXCursor_CXXParenListInitExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXParenListInitExpr);
}

// C++'s reinterpret_cast<> expression.
static Napi::Value Get_CXCursor_CXXReinterpretCastExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXReinterpretCastExpr);
}

// C++'s static_cast<> expression.
static Napi::Value Get_CXCursor_CXXStaticCastExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXStaticCastExpr);
}

// Represents the "this" expression in C++
static Napi::Value Get_CXCursor_CXXThisExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXThisExpr);
}

// [C++ 15] C++ Throw Expression.
// 
// This handles 'throw' and 'throw' assignment-expression. When
// assignment-expression isn't present, Op will be null.
static Napi::Value Get_CXCursor_CXXThrowExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXThrowExpr);
}

// C++'s try statement.
static Napi::Value Get_CXCursor_CXXTryStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXTryStmt);
}

// A C++ typeid expression (C++ [expr.typeid]).
static Napi::Value Get_CXCursor_CXXTypeidExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_CXXTypeidExpr);
}

// An expression that refers to some value declaration, such
// as a function, variable, or enumerator.
static Napi::Value Get_CXCursor_DeclRefExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_DeclRefExpr);
}

// Adaptor class for mixing declarations with statements and
// expressions.
static Napi::Value Get_CXCursor_DeclStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_DeclStmt);
}

// A default statement.
static Napi::Value Get_CXCursor_DefaultStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_DefaultStmt);
}

// A C++ destructor.
static Napi::Value Get_CXCursor_Destructor(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_Destructor);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_DLLExport(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_DLLExport);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_DLLImport(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_DLLImport);
}

// A do statement.
static Napi::Value Get_CXCursor_DoStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_DoStmt);
}

// An enumerator constant.
static Napi::Value Get_CXCursor_EnumConstantDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_EnumConstantDecl);
}

// An enumeration.
static Napi::Value Get_CXCursor_EnumDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_EnumDecl);
}

// A field (in C) or non-static data member (in C++) in a
// struct, union, or C++ class.
static Napi::Value Get_CXCursor_FieldDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_FieldDecl);
}

// Cursor that represents the translation unit itself.
// 
// The translation unit cursor exists primarily to act as the root
// cursor for traversing the contents of a translation unit.
static Napi::Value Get_CXCursor_FirstAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_FirstAttr);
}

// An access specifier.
static Napi::Value Get_CXCursor_FirstDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_FirstDecl);
}

// A reference to a variable that occurs in some non-expression
// context, e.g., a C++ lambda capture list.
static Napi::Value Get_CXCursor_FirstExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_FirstExpr);
}

// a concept declaration.
static Napi::Value Get_CXCursor_FirstExtraDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_FirstExtraDecl);
}

// A reference to a variable that occurs in some non-expression
// context, e.g., a C++ lambda capture list.
static Napi::Value Get_CXCursor_FirstInvalid(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_FirstInvalid);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_FirstPreprocessing(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_FirstPreprocessing);
}

// An access specifier.
static Napi::Value Get_CXCursor_FirstRef(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_FirstRef);
}

// Represents a C++26 pack indexing expression.
static Napi::Value Get_CXCursor_FirstStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_FirstStmt);
}

// Fixed point literal
static Napi::Value Get_CXCursor_FixedPointLiteral(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_FixedPointLiteral);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_FlagEnum(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_FlagEnum);
}

// A floating point number literal.
static Napi::Value Get_CXCursor_FloatingLiteral(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_FloatingLiteral);
}

// A for statement.
static Napi::Value Get_CXCursor_ForStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ForStmt);
}

// a friend declaration.
static Napi::Value Get_CXCursor_FriendDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_FriendDecl);
}

// A function.
static Napi::Value Get_CXCursor_FunctionDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_FunctionDecl);
}

// A C++ function template.
static Napi::Value Get_CXCursor_FunctionTemplate(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_FunctionTemplate);
}

// A GCC inline assembly statement extension.
static Napi::Value Get_CXCursor_GCCAsmStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_GCCAsmStmt);
}

// Represents a C11 generic selection.
static Napi::Value Get_CXCursor_GenericSelectionExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_GenericSelectionExpr);
}

// Implements the GNU __null extension, which is a name for a null
// pointer constant that has integral type (e.g., int or long) and is the same
// size and alignment as a pointer.
// 
// The __null extension is typically only used by system headers, which define
// NULL as __null in C++ rather than using 0 (which is an integer that may not
// match the size of a pointer).
static Napi::Value Get_CXCursor_GNUNullExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_GNUNullExpr);
}

// A goto statement.
static Napi::Value Get_CXCursor_GotoStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_GotoStmt);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_IBActionAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_IBActionAttr);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_IBOutletAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_IBOutletAttr);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_IBOutletCollectionAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_IBOutletCollectionAttr);
}

// An if statement
static Napi::Value Get_CXCursor_IfStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_IfStmt);
}

// An imaginary number literal.
static Napi::Value Get_CXCursor_ImaginaryLiteral(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ImaginaryLiteral);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_InclusionDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_InclusionDirective);
}

// An indirect goto statement.
static Napi::Value Get_CXCursor_IndirectGotoStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_IndirectGotoStmt);
}

// Describes an C or C++ initializer list.
static Napi::Value Get_CXCursor_InitListExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_InitListExpr);
}

// An integer literal.
static Napi::Value Get_CXCursor_IntegerLiteral(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_IntegerLiteral);
}

// A reference to a variable that occurs in some non-expression
// context, e.g., a C++ lambda capture list.
static Napi::Value Get_CXCursor_InvalidCode(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_InvalidCode);
}

// A reference to a variable that occurs in some non-expression
// context, e.g., a C++ lambda capture list.
static Napi::Value Get_CXCursor_InvalidFile(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_InvalidFile);
}

// A reference to a labeled statement.
// 
// This cursor kind is used to describe the jump to "start_over" in the
// goto statement in the following example:
// 
// \code
// start_over:
// ++counter;
// 
// goto start_over;
// \endcode
// 
// A label reference cursor refers to a label statement.
static Napi::Value Get_CXCursor_LabelRef(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_LabelRef);
}

// A labelled statement in a function.
// 
// This cursor kind is used to describe the "start_over:" label statement in
// the following example:
// 
// \code
// start_over:
// ++counter;
// \endcode
static Napi::Value Get_CXCursor_LabelStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_LabelStmt);
}

static Napi::Value Get_CXCursor_LambdaExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_LambdaExpr);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_LastAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_LastAttr);
}

// An access specifier.
static Napi::Value Get_CXCursor_LastDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_LastDecl);
}

// Represents a C++26 pack indexing expression.
static Napi::Value Get_CXCursor_LastExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_LastExpr);
}

// a concept declaration.
static Napi::Value Get_CXCursor_LastExtraDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_LastExtraDecl);
}

// A reference to a variable that occurs in some non-expression
// context, e.g., a C++ lambda capture list.
static Napi::Value Get_CXCursor_LastInvalid(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_LastInvalid);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_LastPreprocessing(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_LastPreprocessing);
}

// A reference to a variable that occurs in some non-expression
// context, e.g., a C++ lambda capture list.
static Napi::Value Get_CXCursor_LastRef(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_LastRef);
}

// OpenACC update Construct.
static Napi::Value Get_CXCursor_LastStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_LastStmt);
}

// A linkage specification, e.g. 'extern "C"'.
static Napi::Value Get_CXCursor_LinkageSpec(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_LinkageSpec);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_MacroDefinition(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_MacroDefinition);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_MacroExpansion(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_MacroExpansion);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_MacroInstantiation(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_MacroInstantiation);
}

// A reference to a member of a struct, union, or class that occurs in
// some non-expression context, e.g., a designated initializer.
static Napi::Value Get_CXCursor_MemberRef(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_MemberRef);
}

// An expression that refers to a member of a struct, union,
// class, Objective-C class, etc.
static Napi::Value Get_CXCursor_MemberRefExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_MemberRefExpr);
}

// A module import declaration.
static Napi::Value Get_CXCursor_ModuleImportDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ModuleImportDecl);
}

// A MS inline assembly statement extension.
static Napi::Value Get_CXCursor_MSAsmStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_MSAsmStmt);
}

// A C++ namespace.
static Napi::Value Get_CXCursor_Namespace(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_Namespace);
}

// A C++ namespace alias declaration.
static Napi::Value Get_CXCursor_NamespaceAlias(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_NamespaceAlias);
}

// A reference to a namespace or namespace alias.
static Napi::Value Get_CXCursor_NamespaceRef(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_NamespaceRef);
}

// A reference to a variable that occurs in some non-expression
// context, e.g., a C++ lambda capture list.
static Napi::Value Get_CXCursor_NoDeclFound(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_NoDeclFound);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_NoDuplicateAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_NoDuplicateAttr);
}

// A C++ non-type template parameter.
static Napi::Value Get_CXCursor_NonTypeTemplateParameter(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_NonTypeTemplateParameter);
}

// A reference to a variable that occurs in some non-expression
// context, e.g., a C++ lambda capture list.
static Napi::Value Get_CXCursor_NotImplemented(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_NotImplemented);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_NSConsumed(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_NSConsumed);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_NSConsumesSelf(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_NSConsumesSelf);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_NSReturnsAutoreleased(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_NSReturnsAutoreleased);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_NSReturnsNotRetained(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_NSReturnsNotRetained);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_NSReturnsRetained(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_NSReturnsRetained);
}

// The null statement ";": C99 6.8.3p3.
// 
// This cursor kind is used to describe the null statement.
static Napi::Value Get_CXCursor_NullStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_NullStmt);
}

// Objective-C's \@catch statement.
static Napi::Value Get_CXCursor_ObjCAtCatchStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCAtCatchStmt);
}

// Objective-C's \@finally statement.
static Napi::Value Get_CXCursor_ObjCAtFinallyStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCAtFinallyStmt);
}

// Objective-C's \@synchronized statement.
static Napi::Value Get_CXCursor_ObjCAtSynchronizedStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCAtSynchronizedStmt);
}

// Objective-C's \@throw statement.
static Napi::Value Get_CXCursor_ObjCAtThrowStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCAtThrowStmt);
}

// Objective-C's overall \@try-\@catch-\@finally statement.
static Napi::Value Get_CXCursor_ObjCAtTryStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCAtTryStmt);
}

// Objective-C's autorelease pool statement.
static Napi::Value Get_CXCursor_ObjCAutoreleasePoolStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCAutoreleasePoolStmt);
}

// Represents an @available(...) check.
static Napi::Value Get_CXCursor_ObjCAvailabilityCheckExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCAvailabilityCheckExpr);
}

// Objective-c Boolean Literal.
static Napi::Value Get_CXCursor_ObjCBoolLiteralExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCBoolLiteralExpr);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_ObjCBoxable(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCBoxable);
}

// An Objective-C "bridged" cast expression, which casts between
// Objective-C pointers and C pointers, transferring ownership in the process.
// 
// \code
// NSString *str = (__bridge_transfer NSString *)CFCreateString();
// \endcode
static Napi::Value Get_CXCursor_ObjCBridgedCastExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCBridgedCastExpr);
}

// An Objective-C \@interface for a category.
static Napi::Value Get_CXCursor_ObjCCategoryDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCCategoryDecl);
}

// An Objective-C \@implementation for a category.
static Napi::Value Get_CXCursor_ObjCCategoryImplDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCCategoryImplDecl);
}

// An Objective-C class method.
static Napi::Value Get_CXCursor_ObjCClassMethodDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCClassMethodDecl);
}

// An access specifier.
static Napi::Value Get_CXCursor_ObjCClassRef(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCClassRef);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_ObjCDesignatedInitializer(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCDesignatedInitializer);
}

// An Objective-C \@dynamic definition.
static Napi::Value Get_CXCursor_ObjCDynamicDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCDynamicDecl);
}

// An Objective-C \@encode expression.
static Napi::Value Get_CXCursor_ObjCEncodeExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCEncodeExpr);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_ObjCException(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCException);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_ObjCExplicitProtocolImpl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCExplicitProtocolImpl);
}

// Objective-C's collection statement.
static Napi::Value Get_CXCursor_ObjCForCollectionStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCForCollectionStmt);
}

// An Objective-C \@implementation.
static Napi::Value Get_CXCursor_ObjCImplementationDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCImplementationDecl);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_ObjCIndependentClass(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCIndependentClass);
}

// An Objective-C instance method.
static Napi::Value Get_CXCursor_ObjCInstanceMethodDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCInstanceMethodDecl);
}

// An Objective-C \@interface.
static Napi::Value Get_CXCursor_ObjCInterfaceDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCInterfaceDecl);
}

// An Objective-C instance variable.
static Napi::Value Get_CXCursor_ObjCIvarDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCIvarDecl);
}

// An expression that sends a message to an Objective-C
// object or class.
static Napi::Value Get_CXCursor_ObjCMessageExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCMessageExpr);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_ObjCNSObject(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCNSObject);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_ObjCPreciseLifetime(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCPreciseLifetime);
}

// An Objective-C \@property declaration.
static Napi::Value Get_CXCursor_ObjCPropertyDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCPropertyDecl);
}

// An Objective-C \@protocol declaration.
static Napi::Value Get_CXCursor_ObjCProtocolDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCProtocolDecl);
}

// An Objective-C \@protocol expression.
static Napi::Value Get_CXCursor_ObjCProtocolExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCProtocolExpr);
}

// An access specifier.
static Napi::Value Get_CXCursor_ObjCProtocolRef(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCProtocolRef);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_ObjCRequiresSuper(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCRequiresSuper);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_ObjCReturnsInnerPointer(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCReturnsInnerPointer);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_ObjCRootClass(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCRootClass);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_ObjCRuntimeVisible(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCRuntimeVisible);
}

// An Objective-C \@selector expression.
static Napi::Value Get_CXCursor_ObjCSelectorExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCSelectorExpr);
}

// Represents the "self" expression in an Objective-C method.
static Napi::Value Get_CXCursor_ObjCSelfExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCSelfExpr);
}

// An Objective-C string literal i.e. @"foo".
static Napi::Value Get_CXCursor_ObjCStringLiteral(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCStringLiteral);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_ObjCSubclassingRestricted(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCSubclassingRestricted);
}

// An access specifier.
static Napi::Value Get_CXCursor_ObjCSuperClassRef(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCSuperClassRef);
}

// An Objective-C \@synthesize definition.
static Napi::Value Get_CXCursor_ObjCSynthesizeDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ObjCSynthesizeDecl);
}

// OpenMP 5.0 [2.1.4, Array Shaping].
static Napi::Value Get_CXCursor_OMPArrayShapingExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPArrayShapingExpr);
}

// OpenMP assume directive.
static Napi::Value Get_CXCursor_OMPAssumeDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPAssumeDirective);
}

// OpenMP atomic directive.
static Napi::Value Get_CXCursor_OMPAtomicDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPAtomicDirective);
}

// OpenMP barrier directive.
static Napi::Value Get_CXCursor_OMPBarrierDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPBarrierDirective);
}

// OpenMP cancel directive.
static Napi::Value Get_CXCursor_OMPCancelDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPCancelDirective);
}

// OpenMP cancellation point directive.
static Napi::Value Get_CXCursor_OMPCancellationPointDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPCancellationPointDirective);
}

// OpenMP canonical loop.
static Napi::Value Get_CXCursor_OMPCanonicalLoop(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPCanonicalLoop);
}

// OpenMP critical directive.
static Napi::Value Get_CXCursor_OMPCriticalDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPCriticalDirective);
}

// OpenMP depobj directive.
static Napi::Value Get_CXCursor_OMPDepobjDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPDepobjDirective);
}

// OpenMP dispatch directive.
static Napi::Value Get_CXCursor_OMPDispatchDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPDispatchDirective);
}

// OpenMP distribute directive.
static Napi::Value Get_CXCursor_OMPDistributeDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPDistributeDirective);
}

// OpenMP distribute parallel for directive.
static Napi::Value Get_CXCursor_OMPDistributeParallelForDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPDistributeParallelForDirective);
}

// OpenMP distribute parallel for simd directive.
static Napi::Value Get_CXCursor_OMPDistributeParallelForSimdDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPDistributeParallelForSimdDirective);
}

// OpenMP distribute simd directive.
static Napi::Value Get_CXCursor_OMPDistributeSimdDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPDistributeSimdDirective);
}

// OpenMP error directive.
static Napi::Value Get_CXCursor_OMPErrorDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPErrorDirective);
}

// OpenMP flush directive.
static Napi::Value Get_CXCursor_OMPFlushDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPFlushDirective);
}

// OpenMP for directive.
static Napi::Value Get_CXCursor_OMPForDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPForDirective);
}

// OpenMP for SIMD directive.
static Napi::Value Get_CXCursor_OMPForSimdDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPForSimdDirective);
}

// OpenMP loop directive.
static Napi::Value Get_CXCursor_OMPGenericLoopDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPGenericLoopDirective);
}

// OpenMP interchange directive.
static Napi::Value Get_CXCursor_OMPInterchangeDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPInterchangeDirective);
}

// OpenMP interop directive.
static Napi::Value Get_CXCursor_OMPInteropDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPInteropDirective);
}

// OpenMP 5.0 [2.1.6 Iterators]
static Napi::Value Get_CXCursor_OMPIteratorExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPIteratorExpr);
}

// OpenMP masked directive.
static Napi::Value Get_CXCursor_OMPMaskedDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPMaskedDirective);
}

// OpenMP masked taskloop directive.
static Napi::Value Get_CXCursor_OMPMaskedTaskLoopDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPMaskedTaskLoopDirective);
}

// OpenMP masked taskloop simd directive.
static Napi::Value Get_CXCursor_OMPMaskedTaskLoopSimdDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPMaskedTaskLoopSimdDirective);
}

// OpenMP master directive.
static Napi::Value Get_CXCursor_OMPMasterDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPMasterDirective);
}

// OpenMP master taskloop directive.
static Napi::Value Get_CXCursor_OMPMasterTaskLoopDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPMasterTaskLoopDirective);
}

// OpenMP master taskloop simd directive.
static Napi::Value Get_CXCursor_OMPMasterTaskLoopSimdDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPMasterTaskLoopSimdDirective);
}

// OpenMP metadirective directive.
static Napi::Value Get_CXCursor_OMPMetaDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPMetaDirective);
}

// OpenMP ordered directive.
static Napi::Value Get_CXCursor_OMPOrderedDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPOrderedDirective);
}

// OpenMP parallel directive.
static Napi::Value Get_CXCursor_OMPParallelDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPParallelDirective);
}

// OpenMP parallel for directive.
static Napi::Value Get_CXCursor_OMPParallelForDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPParallelForDirective);
}

// OpenMP parallel for SIMD directive.
static Napi::Value Get_CXCursor_OMPParallelForSimdDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPParallelForSimdDirective);
}

// OpenMP parallel loop directive.
static Napi::Value Get_CXCursor_OMPParallelGenericLoopDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPParallelGenericLoopDirective);
}

// OpenMP parallel masked directive.
static Napi::Value Get_CXCursor_OMPParallelMaskedDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPParallelMaskedDirective);
}

// OpenMP parallel masked taskloop directive.
static Napi::Value Get_CXCursor_OMPParallelMaskedTaskLoopDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPParallelMaskedTaskLoopDirective);
}

// OpenMP parallel masked taskloop simd directive.
static Napi::Value Get_CXCursor_OMPParallelMaskedTaskLoopSimdDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPParallelMaskedTaskLoopSimdDirective);
}

// OpenMP parallel master directive.
static Napi::Value Get_CXCursor_OMPParallelMasterDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPParallelMasterDirective);
}

// OpenMP parallel master taskloop directive.
static Napi::Value Get_CXCursor_OMPParallelMasterTaskLoopDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPParallelMasterTaskLoopDirective);
}

// OpenMP parallel master taskloop simd directive.
static Napi::Value Get_CXCursor_OMPParallelMasterTaskLoopSimdDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPParallelMasterTaskLoopSimdDirective);
}

// OpenMP parallel sections directive.
static Napi::Value Get_CXCursor_OMPParallelSectionsDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPParallelSectionsDirective);
}

// OpenMP reverse directive.
static Napi::Value Get_CXCursor_OMPReverseDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPReverseDirective);
}

// OpenMP scan directive.
static Napi::Value Get_CXCursor_OMPScanDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPScanDirective);
}

// OpenMP scope directive.
static Napi::Value Get_CXCursor_OMPScopeDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPScopeDirective);
}

// OpenMP section directive.
static Napi::Value Get_CXCursor_OMPSectionDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPSectionDirective);
}

// OpenMP sections directive.
static Napi::Value Get_CXCursor_OMPSectionsDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPSectionsDirective);
}

// OpenMP SIMD directive.
static Napi::Value Get_CXCursor_OMPSimdDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPSimdDirective);
}

// OpenMP single directive.
static Napi::Value Get_CXCursor_OMPSingleDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPSingleDirective);
}

// OpenMP target data directive.
static Napi::Value Get_CXCursor_OMPTargetDataDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTargetDataDirective);
}

// OpenMP target directive.
static Napi::Value Get_CXCursor_OMPTargetDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTargetDirective);
}

// OpenMP target enter data directive.
static Napi::Value Get_CXCursor_OMPTargetEnterDataDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTargetEnterDataDirective);
}

// OpenMP target exit data directive.
static Napi::Value Get_CXCursor_OMPTargetExitDataDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTargetExitDataDirective);
}

// OpenMP target parallel directive.
static Napi::Value Get_CXCursor_OMPTargetParallelDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTargetParallelDirective);
}

// OpenMP target parallel for directive.
static Napi::Value Get_CXCursor_OMPTargetParallelForDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTargetParallelForDirective);
}

// OpenMP target parallel for simd directive.
static Napi::Value Get_CXCursor_OMPTargetParallelForSimdDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTargetParallelForSimdDirective);
}

// OpenMP target parallel loop directive.
static Napi::Value Get_CXCursor_OMPTargetParallelGenericLoopDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTargetParallelGenericLoopDirective);
}

// OpenMP target simd directive.
static Napi::Value Get_CXCursor_OMPTargetSimdDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTargetSimdDirective);
}

// OpenMP target teams directive.
static Napi::Value Get_CXCursor_OMPTargetTeamsDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTargetTeamsDirective);
}

// OpenMP target teams distribute directive.
static Napi::Value Get_CXCursor_OMPTargetTeamsDistributeDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTargetTeamsDistributeDirective);
}

// OpenMP target teams distribute parallel for directive.
static Napi::Value Get_CXCursor_OMPTargetTeamsDistributeParallelForDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTargetTeamsDistributeParallelForDirective);
}

// OpenMP target teams distribute parallel for simd directive.
static Napi::Value Get_CXCursor_OMPTargetTeamsDistributeParallelForSimdDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTargetTeamsDistributeParallelForSimdDirective);
}

// OpenMP target teams distribute simd directive.
static Napi::Value Get_CXCursor_OMPTargetTeamsDistributeSimdDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTargetTeamsDistributeSimdDirective);
}

// OpenMP target teams loop directive.
static Napi::Value Get_CXCursor_OMPTargetTeamsGenericLoopDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTargetTeamsGenericLoopDirective);
}

// OpenMP target update directive.
static Napi::Value Get_CXCursor_OMPTargetUpdateDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTargetUpdateDirective);
}

// OpenMP task directive.
static Napi::Value Get_CXCursor_OMPTaskDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTaskDirective);
}

// OpenMP taskgroup directive.
static Napi::Value Get_CXCursor_OMPTaskgroupDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTaskgroupDirective);
}

// OpenMP taskloop directive.
static Napi::Value Get_CXCursor_OMPTaskLoopDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTaskLoopDirective);
}

// OpenMP taskloop simd directive.
static Napi::Value Get_CXCursor_OMPTaskLoopSimdDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTaskLoopSimdDirective);
}

// OpenMP taskwait directive.
static Napi::Value Get_CXCursor_OMPTaskwaitDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTaskwaitDirective);
}

// OpenMP taskyield directive.
static Napi::Value Get_CXCursor_OMPTaskyieldDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTaskyieldDirective);
}

// OpenMP teams directive.
static Napi::Value Get_CXCursor_OMPTeamsDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTeamsDirective);
}

// OpenMP teams distribute directive.
static Napi::Value Get_CXCursor_OMPTeamsDistributeDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTeamsDistributeDirective);
}

// OpenMP teams distribute parallel for directive.
static Napi::Value Get_CXCursor_OMPTeamsDistributeParallelForDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTeamsDistributeParallelForDirective);
}

// OpenMP teams distribute parallel for simd directive.
static Napi::Value Get_CXCursor_OMPTeamsDistributeParallelForSimdDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTeamsDistributeParallelForSimdDirective);
}

// OpenMP teams distribute simd directive.
static Napi::Value Get_CXCursor_OMPTeamsDistributeSimdDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTeamsDistributeSimdDirective);
}

// OpenMP teams loop directive.
static Napi::Value Get_CXCursor_OMPTeamsGenericLoopDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTeamsGenericLoopDirective);
}

// OpenMP tile directive.
static Napi::Value Get_CXCursor_OMPTileDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPTileDirective);
}

// OpenMP unroll directive.
static Napi::Value Get_CXCursor_OMPUnrollDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OMPUnrollDirective);
}

// OpenACC Combined Constructs.
static Napi::Value Get_CXCursor_OpenACCCombinedConstruct(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OpenACCCombinedConstruct);
}

// OpenACC Compute Construct.
static Napi::Value Get_CXCursor_OpenACCComputeConstruct(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OpenACCComputeConstruct);
}

// OpenACC data Construct.
static Napi::Value Get_CXCursor_OpenACCDataConstruct(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OpenACCDataConstruct);
}

// OpenACC enter data Construct.
static Napi::Value Get_CXCursor_OpenACCEnterDataConstruct(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OpenACCEnterDataConstruct);
}

// OpenACC exit data Construct.
static Napi::Value Get_CXCursor_OpenACCExitDataConstruct(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OpenACCExitDataConstruct);
}

// OpenACC host_data Construct.
static Napi::Value Get_CXCursor_OpenACCHostDataConstruct(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OpenACCHostDataConstruct);
}

// OpenACC init Construct.
static Napi::Value Get_CXCursor_OpenACCInitConstruct(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OpenACCInitConstruct);
}

// OpenACC Loop Construct.
static Napi::Value Get_CXCursor_OpenACCLoopConstruct(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OpenACCLoopConstruct);
}

// OpenACC set Construct.
static Napi::Value Get_CXCursor_OpenACCSetConstruct(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OpenACCSetConstruct);
}

// OpenACC shutdown Construct.
static Napi::Value Get_CXCursor_OpenACCShutdownConstruct(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OpenACCShutdownConstruct);
}

// OpenACC update Construct.
static Napi::Value Get_CXCursor_OpenACCUpdateConstruct(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OpenACCUpdateConstruct);
}

// OpenACC wait Construct.
static Napi::Value Get_CXCursor_OpenACCWaitConstruct(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OpenACCWaitConstruct);
}

// A code completion overload candidate.
static Napi::Value Get_CXCursor_OverloadCandidate(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OverloadCandidate);
}

// A reference to a set of overloaded functions or function templates
// that has not yet been resolved to a specific function or function template.
// 
// An overloaded declaration reference cursor occurs in C++ templates where
// a dependent name refers to a function. For example:
// 
// \code
// template<typename T> void swap(T&, T&);
// 
// struct X { ... };
// void swap(X&, X&);
// 
// template<typename T>
// void reverse(T* first, T* last) {
// while (first < last - 1) {
// swap(*first, *--last);
// ++first;
// }
// }
// 
// struct Y { };
// void swap(Y&, Y&);
// \endcode
// 
// Here, the identifier "swap" is associated with an overloaded declaration
// reference. In the template definition, "swap" refers to either of the two
// "swap" functions declared above, so both results will be available. At
// instantiation time, "swap" may also refer to other functions found via
// argument-dependent lookup (e.g., the "swap" function at the end of the
// example).
// 
// The functions \c clang_getNumOverloadedDecls() and
// \c clang_getOverloadedDecl() can be used to retrieve the definitions
// referenced by this cursor.
static Napi::Value Get_CXCursor_OverloadedDeclRef(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_OverloadedDeclRef);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_PackedAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_PackedAttr);
}

// Represents a C++0x pack expansion that produces a sequence of
// expressions.
// 
// A pack expansion expression contains a pattern (which itself is an
// expression) followed by an ellipsis. For example:
// 
// \code
// template<typename F, typename ...Types>
// void forward(F f, Types &&...args) {
// f(static_cast<Types&&>(args)...);
// }
// \endcode
static Napi::Value Get_CXCursor_PackExpansionExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_PackExpansionExpr);
}

// Represents a C++26 pack indexing expression.
static Napi::Value Get_CXCursor_PackIndexingExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_PackIndexingExpr);
}

// A parenthesized expression, e.g. "(1)".
// 
// This AST node is only formed if full location information is requested.
static Napi::Value Get_CXCursor_ParenExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ParenExpr);
}

// A function or method parameter.
static Napi::Value Get_CXCursor_ParmDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ParmDecl);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_PreprocessingDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_PreprocessingDirective);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_PureAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_PureAttr);
}

// Expression that references a C++20 requires expression.
static Napi::Value Get_CXCursor_RequiresExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_RequiresExpr);
}

// A return statement.
static Napi::Value Get_CXCursor_ReturnStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_ReturnStmt);
}

// Windows Structured Exception Handling's except statement.
static Napi::Value Get_CXCursor_SEHExceptStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_SEHExceptStmt);
}

// Windows Structured Exception Handling's finally statement.
static Napi::Value Get_CXCursor_SEHFinallyStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_SEHFinallyStmt);
}

// Windows Structured Exception Handling's leave statement.
static Napi::Value Get_CXCursor_SEHLeaveStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_SEHLeaveStmt);
}

// Windows Structured Exception Handling's try statement.
static Napi::Value Get_CXCursor_SEHTryStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_SEHTryStmt);
}

// Represents an expression that computes the length of a parameter
// pack.
// 
// \code
// template<typename ...Types>
// struct count {
// static const unsigned value = sizeof...(Types);
// };
// \endcode
static Napi::Value Get_CXCursor_SizeOfPackExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_SizeOfPackExpr);
}

// A static_assert or _Static_assert node
static Napi::Value Get_CXCursor_StaticAssert(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_StaticAssert);
}

// This is the GNU Statement Expression extension: ({int X=4; X;})
static Napi::Value Get_CXCursor_StmtExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_StmtExpr);
}

// A string literal.
static Napi::Value Get_CXCursor_StringLiteral(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_StringLiteral);
}

// A C or C++ struct.
static Napi::Value Get_CXCursor_StructDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_StructDecl);
}

// A switch statement.
static Napi::Value Get_CXCursor_SwitchStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_SwitchStmt);
}

// A reference to a class template, function template, template
// template parameter, or class template partial specialization.
static Napi::Value Get_CXCursor_TemplateRef(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_TemplateRef);
}

// A C++ template template parameter.
static Napi::Value Get_CXCursor_TemplateTemplateParameter(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_TemplateTemplateParameter);
}

// A C++ template type parameter.
static Napi::Value Get_CXCursor_TemplateTypeParameter(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_TemplateTypeParameter);
}

// Cursor that represents the translation unit itself.
// 
// The translation unit cursor exists primarily to act as the root
// cursor for traversing the contents of a translation unit.
static Napi::Value Get_CXCursor_TranslationUnit(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_TranslationUnit);
}

// A C++ alias declaration
static Napi::Value Get_CXCursor_TypeAliasDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_TypeAliasDecl);
}

// A module import declaration.
static Napi::Value Get_CXCursor_TypeAliasTemplateDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_TypeAliasTemplateDecl);
}

// A typedef.
static Napi::Value Get_CXCursor_TypedefDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_TypedefDecl);
}

// A reference to a type declaration.
// 
// A type reference occurs anywhere where a type is named but not
// declared. For example, given:
// 
// \code
// typedef unsigned size_type;
// size_type size;
// \endcode
// 
// The typedef is a declaration of size_type (CXCursor_TypedefDecl),
// while the type of the variable "size" is referenced. The cursor
// referenced by the type of size is the typedef for size_type.
static Napi::Value Get_CXCursor_TypeRef(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_TypeRef);
}

// A unary expression. (noexcept, sizeof, or other traits)
static Napi::Value Get_CXCursor_UnaryExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_UnaryExpr);
}

// This represents the unary-expression's (except sizeof and
// alignof).
static Napi::Value Get_CXCursor_UnaryOperator(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_UnaryOperator);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_UnexposedAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_UnexposedAttr);
}

// A declaration whose specific kind is not exposed via this
// interface.
// 
// Unexposed declarations have the same operations as any other kind
// of declaration; one can extract their location information,
// spelling, find their definitions, etc. However, the specific kind
// of the declaration is not reported.
static Napi::Value Get_CXCursor_UnexposedDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_UnexposedDecl);
}

// An expression whose specific kind is not exposed via this
// interface.
// 
// Unexposed expressions have the same operations as any other kind
// of expression; one can extract their location information,
// spelling, children, etc. However, the specific kind of the
// expression is not reported.
static Napi::Value Get_CXCursor_UnexposedExpr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_UnexposedExpr);
}

// A statement whose specific kind is not exposed via this
// interface.
// 
// Unexposed statements have the same operations as any other kind of
// statement; one can extract their location information, spelling,
// children, etc. However, the specific kind of the statement is not
// reported.
static Napi::Value Get_CXCursor_UnexposedStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_UnexposedStmt);
}

// A C or C++ union.
static Napi::Value Get_CXCursor_UnionDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_UnionDecl);
}

// A C++ using declaration.
static Napi::Value Get_CXCursor_UsingDeclaration(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_UsingDeclaration);
}

// A C++ using directive.
static Napi::Value Get_CXCursor_UsingDirective(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_UsingDirective);
}

// A variable.
static Napi::Value Get_CXCursor_VarDecl(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_VarDecl);
}

// A reference to a variable that occurs in some non-expression
// context, e.g., a C++ lambda capture list.
static Napi::Value Get_CXCursor_VariableRef(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_VariableRef);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_VisibilityAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_VisibilityAttr);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_WarnUnusedAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_WarnUnusedAttr);
}

// An attribute whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXCursor_WarnUnusedResultAttr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_WarnUnusedResultAttr);
}

// A while statement.
static Napi::Value Get_CXCursor_WhileStmt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXCursor_WhileStmt);
}

// Options to control the display of diagnostics.
// 
// The values in this enum are meant to be combined to customize the
// behavior of \c clang_formatDiagnostic().
// enum CXDiagnosticDisplayOptions
// Display the category number associated with this diagnostic, if any.
// 
// The category number is displayed within brackets after the diagnostic text.
// This option corresponds to the clang flag
// \c -fdiagnostics-show-category=id.
static Napi::Value Get_CXDiagnostic_DisplayCategoryId(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXDiagnostic_DisplayCategoryId);
}

// Display the category name associated with this diagnostic, if any.
// 
// The category name is displayed within brackets after the diagnostic text.
// This option corresponds to the clang flag
// \c -fdiagnostics-show-category=name.
static Napi::Value Get_CXDiagnostic_DisplayCategoryName(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXDiagnostic_DisplayCategoryName);
}

// If displaying the source-location information of the
// diagnostic, also include the column number.
// 
// This option corresponds to the clang flag \c -fshow-column.
static Napi::Value Get_CXDiagnostic_DisplayColumn(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXDiagnostic_DisplayColumn);
}

// Display the option name associated with this diagnostic, if any.
// 
// The option name displayed (e.g., -Wconversion) will be placed in brackets
// after the diagnostic text. This option corresponds to the clang flag
// \c -fdiagnostics-show-option.
static Napi::Value Get_CXDiagnostic_DisplayOption(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXDiagnostic_DisplayOption);
}

// Display the source-location information where the
// diagnostic was located.
// 
// When set, diagnostics will be prefixed by the file, line, and
// (optionally) column to which the diagnostic refers. For example,
// 
// \code
// test.c:28: warning: extra tokens at end of #endif directive
// \endcode
// 
// This option corresponds to the clang flag \c -fshow-source-location.
static Napi::Value Get_CXDiagnostic_DisplaySourceLocation(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXDiagnostic_DisplaySourceLocation);
}

// If displaying the source-location information of the
// diagnostic, also include information about source ranges in a
// machine-parsable format.
// 
// This option corresponds to the clang flag
// \c -fdiagnostics-print-source-range-info.
static Napi::Value Get_CXDiagnostic_DisplaySourceRanges(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXDiagnostic_DisplaySourceRanges);
}

// Describes the severity of a particular diagnostic.
// enum CXDiagnosticSeverity
// This diagnostic indicates that the code is ill-formed.
static Napi::Value Get_CXDiagnostic_Error(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXDiagnostic_Error);
}

// This diagnostic indicates that the code is ill-formed such
// that future parser recovery is unlikely to produce useful
// results.
static Napi::Value Get_CXDiagnostic_Fatal(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXDiagnostic_Fatal);
}

// A diagnostic that has been suppressed, e.g., by a command-line
// option.
static Napi::Value Get_CXDiagnostic_Ignored(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXDiagnostic_Ignored);
}

// This diagnostic is a note that should be attached to the
// previous (non-note) diagnostic.
static Napi::Value Get_CXDiagnostic_Note(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXDiagnostic_Note);
}

// This diagnostic indicates suspicious code that may not be
// wrong.
static Napi::Value Get_CXDiagnostic_Warning(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXDiagnostic_Warning);
}

// Error codes returned by libclang routines.
// 
// Zero (\c CXError_Success) is the only error code indicating success.  Other
// error codes, including not yet assigned non-zero values, indicate errors.
// enum CXErrorCode
// An AST deserialization error has occurred.
static Napi::Value Get_CXError_ASTReadError(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXError_ASTReadError);
}

// libclang crashed while performing the requested operation.
static Napi::Value Get_CXError_Crashed(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXError_Crashed);
}

// A generic error code, no further details are available.
// 
// Errors of this kind can get their own specific error codes in future
// libclang versions.
static Napi::Value Get_CXError_Failure(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXError_Failure);
}

// The function detected that the arguments violate the function
// contract.
static Napi::Value Get_CXError_InvalidArguments(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXError_InvalidArguments);
}

// No error.
static Napi::Value Get_CXError_Success(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXError_Success);
}

// enum CXEvalResultKind
static Napi::Value Get_CXEval_CFStr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXEval_CFStr);
}

static Napi::Value Get_CXEval_Float(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXEval_Float);
}

static Napi::Value Get_CXEval_Int(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXEval_Int);
}

static Napi::Value Get_CXEval_ObjCStrLiteral(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXEval_ObjCStrLiteral);
}

static Napi::Value Get_CXEval_Other(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXEval_Other);
}

static Napi::Value Get_CXEval_StrLiteral(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXEval_StrLiteral);
}

static Napi::Value Get_CXEval_UnExposed(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXEval_UnExposed);
}

// enum CXEvalResultKind
// enum CXGlobalOptFlags
// Used to indicate that no special CXIndex options are needed.
static Napi::Value Get_CXGlobalOpt_None(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXGlobalOpt_None);
}

// Used to indicate that all threads that libclang creates should use
// background priority.
static Napi::Value Get_CXGlobalOpt_ThreadBackgroundPriorityForAll(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXGlobalOpt_ThreadBackgroundPriorityForAll);
}

// Used to indicate that threads that libclang creates for editing
// purposes should use background priority.
// 
// Affects #clang_reparseTranslationUnit, #clang_codeCompleteAt,
// #clang_annotateTokens
static Napi::Value Get_CXGlobalOpt_ThreadBackgroundPriorityForEditing(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXGlobalOpt_ThreadBackgroundPriorityForEditing);
}

// Used to indicate that threads that libclang creates for indexing
// purposes should use background priority.
// 
// Affects #clang_indexSourceFile, #clang_indexTranslationUnit,
// #clang_parseTranslationUnit, #clang_saveTranslationUnit.
static Napi::Value Get_CXGlobalOpt_ThreadBackgroundPriorityForIndexing(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXGlobalOpt_ThreadBackgroundPriorityForIndexing);
}

// enum CXGlobalOptFlags
// enum CXIdxAttrKind
static Napi::Value Get_CXIdxAttr_IBAction(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxAttr_IBAction);
}

static Napi::Value Get_CXIdxAttr_IBOutlet(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxAttr_IBOutlet);
}

static Napi::Value Get_CXIdxAttr_IBOutletCollection(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxAttr_IBOutletCollection);
}

static Napi::Value Get_CXIdxAttr_Unexposed(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxAttr_Unexposed);
}

// enum CXIdxAttrKind
// enum CXIdxDeclInfoFlags
static Napi::Value Get_CXIdxDeclFlag_Skipped(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxDeclFlag_Skipped);
}

// enum CXIdxDeclInfoFlags
// Extra C++ template information for an entity. This can apply to:
// CXIdxEntity_Function
// CXIdxEntity_CXXClass
// CXIdxEntity_CXXStaticMethod
// CXIdxEntity_CXXInstanceMethod
// CXIdxEntity_CXXConstructor
// CXIdxEntity_CXXConversionFunction
// CXIdxEntity_CXXTypeAlias
// enum CXIdxEntityCXXTemplateKind
static Napi::Value Get_CXIdxEntity_NonTemplate(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_NonTemplate);
}

static Napi::Value Get_CXIdxEntity_Template(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_Template);
}

static Napi::Value Get_CXIdxEntity_TemplatePartialSpecialization(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_TemplatePartialSpecialization);
}

static Napi::Value Get_CXIdxEntity_TemplateSpecialization(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_TemplateSpecialization);
}

// Extra C++ template information for an entity. This can apply to:
// CXIdxEntity_Function
// CXIdxEntity_CXXClass
// CXIdxEntity_CXXStaticMethod
// CXIdxEntity_CXXInstanceMethod
// CXIdxEntity_CXXConstructor
// CXIdxEntity_CXXConversionFunction
// CXIdxEntity_CXXTypeAlias
// enum CXIdxEntityCXXTemplateKind
// enum CXIdxEntityKind
static Napi::Value Get_CXIdxEntity_CXXClass(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_CXXClass);
}

static Napi::Value Get_CXIdxEntity_CXXConcept(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_CXXConcept);
}

static Napi::Value Get_CXIdxEntity_CXXConstructor(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_CXXConstructor);
}

static Napi::Value Get_CXIdxEntity_CXXConversionFunction(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_CXXConversionFunction);
}

static Napi::Value Get_CXIdxEntity_CXXDestructor(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_CXXDestructor);
}

static Napi::Value Get_CXIdxEntity_CXXInstanceMethod(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_CXXInstanceMethod);
}

static Napi::Value Get_CXIdxEntity_CXXInterface(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_CXXInterface);
}

static Napi::Value Get_CXIdxEntity_CXXNamespace(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_CXXNamespace);
}

static Napi::Value Get_CXIdxEntity_CXXNamespaceAlias(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_CXXNamespaceAlias);
}

static Napi::Value Get_CXIdxEntity_CXXStaticMethod(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_CXXStaticMethod);
}

static Napi::Value Get_CXIdxEntity_CXXStaticVariable(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_CXXStaticVariable);
}

static Napi::Value Get_CXIdxEntity_CXXTypeAlias(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_CXXTypeAlias);
}

static Napi::Value Get_CXIdxEntity_Enum(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_Enum);
}

static Napi::Value Get_CXIdxEntity_EnumConstant(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_EnumConstant);
}

static Napi::Value Get_CXIdxEntity_Field(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_Field);
}

static Napi::Value Get_CXIdxEntity_Function(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_Function);
}

static Napi::Value Get_CXIdxEntity_ObjCCategory(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_ObjCCategory);
}

static Napi::Value Get_CXIdxEntity_ObjCClass(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_ObjCClass);
}

static Napi::Value Get_CXIdxEntity_ObjCClassMethod(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_ObjCClassMethod);
}

static Napi::Value Get_CXIdxEntity_ObjCInstanceMethod(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_ObjCInstanceMethod);
}

static Napi::Value Get_CXIdxEntity_ObjCIvar(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_ObjCIvar);
}

static Napi::Value Get_CXIdxEntity_ObjCProperty(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_ObjCProperty);
}

static Napi::Value Get_CXIdxEntity_ObjCProtocol(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_ObjCProtocol);
}

static Napi::Value Get_CXIdxEntity_Struct(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_Struct);
}

static Napi::Value Get_CXIdxEntity_Typedef(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_Typedef);
}

static Napi::Value Get_CXIdxEntity_Unexposed(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_Unexposed);
}

static Napi::Value Get_CXIdxEntity_Union(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_Union);
}

static Napi::Value Get_CXIdxEntity_Variable(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntity_Variable);
}

// enum CXIdxEntityKind
// enum CXIdxEntityLanguage
static Napi::Value Get_CXIdxEntityLang_C(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntityLang_C);
}

static Napi::Value Get_CXIdxEntityLang_CXX(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntityLang_CXX);
}

static Napi::Value Get_CXIdxEntityLang_None(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntityLang_None);
}

static Napi::Value Get_CXIdxEntityLang_ObjC(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntityLang_ObjC);
}

static Napi::Value Get_CXIdxEntityLang_Swift(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntityLang_Swift);
}

// enum CXIdxEntityLanguage
// Data for IndexerCallbacks#indexEntityReference.
// 
// This may be deprecated in a future version as this duplicates
// the \c CXSymbolRole_Implicit bit in \c CXSymbolRole.
// enum CXIdxEntityRefKind
// The entity is referenced directly in user's code.
static Napi::Value Get_CXIdxEntityRef_Direct(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntityRef_Direct);
}

// An implicit reference, e.g. a reference of an Objective-C method
// via the dot syntax.
static Napi::Value Get_CXIdxEntityRef_Implicit(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxEntityRef_Implicit);
}

// Data for IndexerCallbacks#indexEntityReference.
// 
// This may be deprecated in a future version as this duplicates
// the \c CXSymbolRole_Implicit bit in \c CXSymbolRole.
// enum CXIdxEntityRefKind
// enum CXIdxObjCContainerKind
static Napi::Value Get_CXIdxObjCContainer_ForwardRef(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxObjCContainer_ForwardRef);
}

static Napi::Value Get_CXIdxObjCContainer_Implementation(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxObjCContainer_Implementation);
}

static Napi::Value Get_CXIdxObjCContainer_Interface(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIdxObjCContainer_Interface);
}

// enum CXIdxObjCContainerKind
// enum CXIndexOptFlags
// Function-local symbols should be indexed. If this is not set
// function-local symbols will be ignored.
static Napi::Value Get_CXIndexOpt_IndexFunctionLocalSymbols(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIndexOpt_IndexFunctionLocalSymbols);
}

// Implicit function/class template instantiations should be indexed.
// If this is not set, implicit instantiations will be ignored.
static Napi::Value Get_CXIndexOpt_IndexImplicitTemplateInstantiations(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIndexOpt_IndexImplicitTemplateInstantiations);
}

// Used to indicate that no special indexing options are needed.
static Napi::Value Get_CXIndexOpt_None(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIndexOpt_None);
}

// Skip a function/method body that was already parsed during an
// indexing session associated with a \c CXIndexAction object.
// Bodies in system headers are always skipped.
static Napi::Value Get_CXIndexOpt_SkipParsedBodiesInSession(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIndexOpt_SkipParsedBodiesInSession);
}

// Used to indicate that IndexerCallbacks#indexEntityReference should
// be invoked for only one reference of an entity per source file that does
// not also include a declaration/definition of the entity.
static Napi::Value Get_CXIndexOpt_SuppressRedundantRefs(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIndexOpt_SuppressRedundantRefs);
}

// Suppress all compiler warnings when parsing for indexing.
static Napi::Value Get_CXIndexOpt_SuppressWarnings(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXIndexOpt_SuppressWarnings);
}

// enum CXIndexOptFlags
// Describe the "language" of the entity referred to by a cursor.
// enum CXLanguageKind
static Napi::Value Get_CXLanguage_C(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXLanguage_C);
}

static Napi::Value Get_CXLanguage_CPlusPlus(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXLanguage_CPlusPlus);
}

static Napi::Value Get_CXLanguage_Invalid(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXLanguage_Invalid);
}

static Napi::Value Get_CXLanguage_ObjC(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXLanguage_ObjC);
}

// Describe the linkage of the entity referred to by a cursor.
// enum CXLinkageKind
// This is the linkage for entities with true, external linkage.
static Napi::Value Get_CXLinkage_External(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXLinkage_External);
}

// This is the linkage for static variables and static functions.
static Napi::Value Get_CXLinkage_Internal(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXLinkage_Internal);
}

// This value indicates that no linkage information is available
// for a provided CXCursor.
static Napi::Value Get_CXLinkage_Invalid(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXLinkage_Invalid);
}

// This is the linkage for variables, parameters, and so on that
// have automatic storage.  This covers normal (non-extern) local variables.
static Napi::Value Get_CXLinkage_NoLinkage(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXLinkage_NoLinkage);
}

// This is the linkage for entities with external linkage that live
// in C++ anonymous namespaces.
static Napi::Value Get_CXLinkage_UniqueExternal(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXLinkage_UniqueExternal);
}

// Describes the kind of error that occurred (if any) in a call to
// \c clang_loadDiagnostics.
// enum CXLoadDiag_Error
// Indicates that the file containing the serialized diagnostics
// could not be opened.
static Napi::Value Get_CXLoadDiag_CannotLoad(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXLoadDiag_CannotLoad);
}

// Indicates that the serialized diagnostics file is invalid or
// corrupt.
static Napi::Value Get_CXLoadDiag_InvalidFile(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXLoadDiag_InvalidFile);
}

// Indicates that no error occurred.
static Napi::Value Get_CXLoadDiag_None(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXLoadDiag_None);
}

// Indicates that an unknown error occurred while attempting to
// deserialize diagnostics.
static Napi::Value Get_CXLoadDiag_Unknown(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXLoadDiag_Unknown);
}

// enum CXNameRefFlags
// Include the nested-name-specifier, e.g. Foo:: in x.Foo::y, in the
// range.
static Napi::Value Get_CXNameRange_WantQualifier(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXNameRange_WantQualifier);
}

// If the name is non-contiguous, return the full spanning range.
// 
// Non-contiguous names occur in Objective-C when a selector with two or more
// parameters is used, or in C++ when using an operator:
// \code
// [object doSomething:here withValue:there]; // Objective-C
// return some_vector[1]; // C++
// \endcode
static Napi::Value Get_CXNameRange_WantSinglePiece(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXNameRange_WantSinglePiece);
}

// Include the explicit template arguments, e.g. \<int> in x.f<int>,
// in the range.
static Napi::Value Get_CXNameRange_WantTemplateArgs(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXNameRange_WantTemplateArgs);
}

// 'Qualifiers' written next to the return and parameter types in
// Objective-C method declarations.
// enum CXObjCDeclQualifierKind
static Napi::Value Get_CXObjCDeclQualifier_Bycopy(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCDeclQualifier_Bycopy);
}

static Napi::Value Get_CXObjCDeclQualifier_Byref(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCDeclQualifier_Byref);
}

static Napi::Value Get_CXObjCDeclQualifier_In(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCDeclQualifier_In);
}

static Napi::Value Get_CXObjCDeclQualifier_Inout(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCDeclQualifier_Inout);
}

static Napi::Value Get_CXObjCDeclQualifier_None(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCDeclQualifier_None);
}

static Napi::Value Get_CXObjCDeclQualifier_Oneway(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCDeclQualifier_Oneway);
}

static Napi::Value Get_CXObjCDeclQualifier_Out(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCDeclQualifier_Out);
}

// 'Qualifiers' written next to the return and parameter types in
// Objective-C method declarations.
// enum CXObjCDeclQualifierKind
// Property attributes for a \c CXCursor_ObjCPropertyDecl.
// enum CXObjCPropertyAttrKind
static Napi::Value Get_CXObjCPropertyAttr_assign(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCPropertyAttr_assign);
}

static Napi::Value Get_CXObjCPropertyAttr_atomic(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCPropertyAttr_atomic);
}

static Napi::Value Get_CXObjCPropertyAttr_class(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCPropertyAttr_class);
}

static Napi::Value Get_CXObjCPropertyAttr_copy(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCPropertyAttr_copy);
}

static Napi::Value Get_CXObjCPropertyAttr_getter(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCPropertyAttr_getter);
}

static Napi::Value Get_CXObjCPropertyAttr_noattr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCPropertyAttr_noattr);
}

static Napi::Value Get_CXObjCPropertyAttr_nonatomic(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCPropertyAttr_nonatomic);
}

static Napi::Value Get_CXObjCPropertyAttr_readonly(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCPropertyAttr_readonly);
}

static Napi::Value Get_CXObjCPropertyAttr_readwrite(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCPropertyAttr_readwrite);
}

static Napi::Value Get_CXObjCPropertyAttr_retain(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCPropertyAttr_retain);
}

static Napi::Value Get_CXObjCPropertyAttr_setter(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCPropertyAttr_setter);
}

static Napi::Value Get_CXObjCPropertyAttr_strong(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCPropertyAttr_strong);
}

static Napi::Value Get_CXObjCPropertyAttr_unsafe_unretained(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCPropertyAttr_unsafe_unretained);
}

static Napi::Value Get_CXObjCPropertyAttr_weak(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXObjCPropertyAttr_weak);
}

// Property attributes for a \c CXCursor_ObjCPropertyDecl.
// enum CXObjCPropertyAttrKind
// Properties for the printing policy.
// 
// See \c clang::PrintingPolicy for more information.
// enum CXPrintingPolicyProperty
static Napi::Value Get_CXPrintingPolicy_Alignof(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_Alignof);
}

static Napi::Value Get_CXPrintingPolicy_AnonymousTagLocations(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_AnonymousTagLocations);
}

static Napi::Value Get_CXPrintingPolicy_Bool(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_Bool);
}

static Napi::Value Get_CXPrintingPolicy_ConstantArraySizeAsWritten(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_ConstantArraySizeAsWritten);
}

static Napi::Value Get_CXPrintingPolicy_ConstantsAsWritten(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_ConstantsAsWritten);
}

static Napi::Value Get_CXPrintingPolicy_FullyQualifiedName(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_FullyQualifiedName);
}

static Napi::Value Get_CXPrintingPolicy_Half(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_Half);
}

static Napi::Value Get_CXPrintingPolicy_IncludeNewlines(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_IncludeNewlines);
}

static Napi::Value Get_CXPrintingPolicy_IncludeTagDefinition(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_IncludeTagDefinition);
}

static Napi::Value Get_CXPrintingPolicy_Indentation(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_Indentation);
}

static Napi::Value Get_CXPrintingPolicy_LastProperty(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_LastProperty);
}

static Napi::Value Get_CXPrintingPolicy_MSVCFormatting(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_MSVCFormatting);
}

static Napi::Value Get_CXPrintingPolicy_MSWChar(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_MSWChar);
}

static Napi::Value Get_CXPrintingPolicy_PolishForDeclaration(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_PolishForDeclaration);
}

static Napi::Value Get_CXPrintingPolicy_Restrict(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_Restrict);
}

static Napi::Value Get_CXPrintingPolicy_SuppressImplicitBase(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_SuppressImplicitBase);
}

static Napi::Value Get_CXPrintingPolicy_SuppressInitializers(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_SuppressInitializers);
}

static Napi::Value Get_CXPrintingPolicy_SuppressLifetimeQualifiers(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_SuppressLifetimeQualifiers);
}

static Napi::Value Get_CXPrintingPolicy_SuppressScope(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_SuppressScope);
}

static Napi::Value Get_CXPrintingPolicy_SuppressSpecifiers(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_SuppressSpecifiers);
}

static Napi::Value Get_CXPrintingPolicy_SuppressStrongLifetime(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_SuppressStrongLifetime);
}

static Napi::Value Get_CXPrintingPolicy_SuppressTagKeyword(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_SuppressTagKeyword);
}

static Napi::Value Get_CXPrintingPolicy_SuppressTemplateArgsInCXXConstructors(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_SuppressTemplateArgsInCXXConstructors);
}

static Napi::Value Get_CXPrintingPolicy_SuppressUnwrittenScope(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_SuppressUnwrittenScope);
}

static Napi::Value Get_CXPrintingPolicy_TerseOutput(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_TerseOutput);
}

static Napi::Value Get_CXPrintingPolicy_UnderscoreAlignof(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_UnderscoreAlignof);
}

static Napi::Value Get_CXPrintingPolicy_UseVoidForZeroParams(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXPrintingPolicy_UseVoidForZeroParams);
}

// enum CXRefQualifierKind
// An lvalue ref-qualifier was provided (\c &).
static Napi::Value Get_CXRefQualifier_LValue(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXRefQualifier_LValue);
}

// No ref-qualifier was provided.
static Napi::Value Get_CXRefQualifier_None(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXRefQualifier_None);
}

// An rvalue ref-qualifier was provided (\c &&).
static Napi::Value Get_CXRefQualifier_RValue(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXRefQualifier_RValue);
}

// Flags that control the reparsing of translation units.
// 
// The enumerators in this enumeration type are meant to be bitwise
// ORed together to specify which options should be used when
// reparsing the translation unit.
// enum CXReparse_Flags
// Used to indicate that no special reparsing options are needed.
static Napi::Value Get_CXReparse_None(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXReparse_None);
}

// enum CXResult
// One of the parameters was invalid for the function.
static Napi::Value Get_CXResult_Invalid(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXResult_Invalid);
}

// Function returned successfully.
static Napi::Value Get_CXResult_Success(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXResult_Success);
}

// The function was terminated by a callback (e.g. it returned
// CXVisit_Break)
static Napi::Value Get_CXResult_VisitBreak(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXResult_VisitBreak);
}

// enum CXResult
// Describes the kind of error that occurred (if any) in a call to
// \c clang_saveTranslationUnit().
// enum CXSaveError
// Indicates that the translation unit to be saved was somehow
// invalid (e.g., NULL).
static Napi::Value Get_CXSaveError_InvalidTU(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXSaveError_InvalidTU);
}

// Indicates that no error occurred while saving a translation unit.
static Napi::Value Get_CXSaveError_None(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXSaveError_None);
}

// Indicates that errors during translation prevented this attempt
// to save the translation unit.
// 
// Errors that prevent the translation unit from being saved can be
// extracted using \c clang_getNumDiagnostics() and \c clang_getDiagnostic().
static Napi::Value Get_CXSaveError_TranslationErrors(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXSaveError_TranslationErrors);
}

// Indicates that an unknown error occurred while attempting to save
// the file.
// 
// This error typically indicates that file I/O failed when attempting to
// write the file.
static Napi::Value Get_CXSaveError_Unknown(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXSaveError_Unknown);
}

// Flags that control how translation units are saved.
// 
// The enumerators in this enumeration type are meant to be bitwise
// ORed together to specify which options should be used when
// saving the translation unit.
// enum CXSaveTranslationUnit_Flags
// Used to indicate that no special saving options are needed.
static Napi::Value Get_CXSaveTranslationUnit_None(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXSaveTranslationUnit_None);
}

// Roles that are attributed to symbol occurrences.
// 
// Internal: this currently mirrors low 9 bits of clang::index::SymbolRole with
// higher bits zeroed. These high bits may be exposed in the future.
// enum CXSymbolRole
static Napi::Value Get_CXSymbolRole_AddressOf(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXSymbolRole_AddressOf);
}

static Napi::Value Get_CXSymbolRole_Call(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXSymbolRole_Call);
}

static Napi::Value Get_CXSymbolRole_Declaration(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXSymbolRole_Declaration);
}

static Napi::Value Get_CXSymbolRole_Definition(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXSymbolRole_Definition);
}

static Napi::Value Get_CXSymbolRole_Dynamic(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXSymbolRole_Dynamic);
}

static Napi::Value Get_CXSymbolRole_Implicit(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXSymbolRole_Implicit);
}

static Napi::Value Get_CXSymbolRole_None(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXSymbolRole_None);
}

static Napi::Value Get_CXSymbolRole_Read(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXSymbolRole_Read);
}

static Napi::Value Get_CXSymbolRole_Reference(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXSymbolRole_Reference);
}

static Napi::Value Get_CXSymbolRole_Write(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXSymbolRole_Write);
}

// Roles that are attributed to symbol occurrences.
// 
// Internal: this currently mirrors low 9 bits of clang::index::SymbolRole with
// higher bits zeroed. These high bits may be exposed in the future.
// enum CXSymbolRole
// Describes the kind of a template argument.
// 
// See the definition of llvm::clang::TemplateArgument::ArgKind for full
// element descriptions.
// enum CXTemplateArgumentKind
static Napi::Value Get_CXTemplateArgumentKind_Declaration(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTemplateArgumentKind_Declaration);
}

static Napi::Value Get_CXTemplateArgumentKind_Expression(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTemplateArgumentKind_Expression);
}

static Napi::Value Get_CXTemplateArgumentKind_Integral(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTemplateArgumentKind_Integral);
}

static Napi::Value Get_CXTemplateArgumentKind_Invalid(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTemplateArgumentKind_Invalid);
}

static Napi::Value Get_CXTemplateArgumentKind_Null(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTemplateArgumentKind_Null);
}

static Napi::Value Get_CXTemplateArgumentKind_NullPtr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTemplateArgumentKind_NullPtr);
}

static Napi::Value Get_CXTemplateArgumentKind_Pack(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTemplateArgumentKind_Pack);
}

static Napi::Value Get_CXTemplateArgumentKind_Template(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTemplateArgumentKind_Template);
}

static Napi::Value Get_CXTemplateArgumentKind_TemplateExpansion(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTemplateArgumentKind_TemplateExpansion);
}

static Napi::Value Get_CXTemplateArgumentKind_Type(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTemplateArgumentKind_Type);
}

// Describe the "thread-local storage (TLS) kind" of the declaration
// referred to by a cursor.
// enum CXTLSKind
static Napi::Value Get_CXTLS_Dynamic(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTLS_Dynamic);
}

static Napi::Value Get_CXTLS_None(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTLS_None);
}

static Napi::Value Get_CXTLS_Static(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTLS_Static);
}

// Describes a kind of token.
// enum CXTokenKind
// A comment.
static Napi::Value Get_CXToken_Comment(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXToken_Comment);
}

// An identifier (that is not a keyword).
static Napi::Value Get_CXToken_Identifier(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXToken_Identifier);
}

// A language keyword.
static Napi::Value Get_CXToken_Keyword(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXToken_Keyword);
}

// A numeric, string, or character literal.
static Napi::Value Get_CXToken_Literal(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXToken_Literal);
}

// A token that contains some kind of punctuation.
static Napi::Value Get_CXToken_Punctuation(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXToken_Punctuation);
}

// Describes a kind of token.
// enum CXTokenKind
// Flags that control the creation of translation units.
// 
// The enumerators in this enumeration type are meant to be bitwise
// ORed together to specify which options should be used when
// constructing the translation unit.
// enum CXTranslationUnit_Flags
// Used to indicate that the translation unit should cache some
// code-completion results with each reparse of the source file.
// 
// Caching of code-completion results is a performance optimization that
// introduces some overhead to reparsing but improves the performance of
// code-completion operations.
static Napi::Value Get_CXTranslationUnit_CacheCompletionResults(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTranslationUnit_CacheCompletionResults);
}

// Used to indicate that the precompiled preamble should be created on
// the first parse. Otherwise it will be created on the first reparse. This
// trades runtime on the first parse (serializing the preamble takes time) for
// reduced runtime on the second parse (can now reuse the preamble).
static Napi::Value Get_CXTranslationUnit_CreatePreambleOnFirstParse(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTranslationUnit_CreatePreambleOnFirstParse);
}

// DEPRECATED: Enabled chained precompiled preambles in C++.
// 
// Note: this is a *temporary* option that is available only while
// we are testing C++ precompiled preamble support. It is deprecated.
static Napi::Value Get_CXTranslationUnit_CXXChainedPCH(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTranslationUnit_CXXChainedPCH);
}

// Used to indicate that the parser should construct a "detailed"
// preprocessing record, including all macro definitions and instantiations.
// 
// Constructing a detailed preprocessing record requires more memory
// and time to parse, since the information contained in the record
// is usually not retained. However, it can be useful for
// applications that require more detailed information about the
// behavior of the preprocessor.
static Napi::Value Get_CXTranslationUnit_DetailedPreprocessingRecord(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTranslationUnit_DetailedPreprocessingRecord);
}

// Used to indicate that the translation unit will be serialized with
// \c clang_saveTranslationUnit.
// 
// This option is typically used when parsing a header with the intent of
// producing a precompiled header.
static Napi::Value Get_CXTranslationUnit_ForSerialization(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTranslationUnit_ForSerialization);
}

// Used to indicate that non-errors from included files should be ignored.
// 
// If set, clang_getDiagnosticSetFromTU() will not report e.g. warnings from
// included files anymore. This speeds up clang_getDiagnosticSetFromTU() for
// the case where these warnings are not of interest, as for an IDE for
// example, which typically shows only the diagnostics in the main file.
static Napi::Value Get_CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles);
}

// Used to indicate that attributed types should be included in CXType.
static Napi::Value Get_CXTranslationUnit_IncludeAttributedTypes(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTranslationUnit_IncludeAttributedTypes);
}

// Used to indicate that brief documentation comments should be
// included into the set of code completions returned from this translation
// unit.
static Napi::Value Get_CXTranslationUnit_IncludeBriefCommentsInCodeCompletion(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTranslationUnit_IncludeBriefCommentsInCodeCompletion);
}

// Used to indicate that the translation unit is incomplete.
// 
// When a translation unit is considered "incomplete", semantic
// analysis that is typically performed at the end of the
// translation unit will be suppressed. For example, this suppresses
// the completion of tentative declarations in C and of
// instantiation of implicitly-instantiation function templates in
// C++. This option is typically used when parsing a header with the
// intent of producing a precompiled header.
static Napi::Value Get_CXTranslationUnit_Incomplete(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTranslationUnit_Incomplete);
}

// Do not stop processing when fatal errors are encountered.
// 
// When fatal errors are encountered while parsing a translation unit,
// semantic analysis is typically stopped early when compiling code. A common
// source for fatal errors are unresolvable include files. For the
// purposes of an IDE, this is undesirable behavior and as much information
// as possible should be reported. Use this flag to enable this behavior.
static Napi::Value Get_CXTranslationUnit_KeepGoing(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTranslationUnit_KeepGoing);
}

// Used in combination with CXTranslationUnit_SkipFunctionBodies to
// constrain the skipping of function bodies to the preamble.
// 
// The function bodies of the main file are not skipped.
static Napi::Value Get_CXTranslationUnit_LimitSkipFunctionBodiesToPreamble(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTranslationUnit_LimitSkipFunctionBodiesToPreamble);
}

// Used to indicate that no special translation-unit options are
// needed.
static Napi::Value Get_CXTranslationUnit_None(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTranslationUnit_None);
}

// Used to indicate that the translation unit should be built with an
// implicit precompiled header for the preamble.
// 
// An implicit precompiled header is used as an optimization when a
// particular translation unit is likely to be reparsed many times
// when the sources aren't changing that often. In this case, an
// implicit precompiled header will be built containing all of the
// initial includes at the top of the main file (what we refer to as
// the "preamble" of the file). In subsequent parses, if the
// preamble or the files in it have not changed, \c
// clang_reparseTranslationUnit() will re-use the implicit
// precompiled header to improve parsing performance.
static Napi::Value Get_CXTranslationUnit_PrecompiledPreamble(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTranslationUnit_PrecompiledPreamble);
}

// Tells the preprocessor not to skip excluded conditional blocks.
static Napi::Value Get_CXTranslationUnit_RetainExcludedConditionalBlocks(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTranslationUnit_RetainExcludedConditionalBlocks);
}

// Sets the preprocessor in a mode for parsing a single file only.
static Napi::Value Get_CXTranslationUnit_SingleFileParse(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTranslationUnit_SingleFileParse);
}

// Used to indicate that function/method bodies should be skipped while
// parsing.
// 
// This option can be used to search for declarations/definitions while
// ignoring the usages.
static Napi::Value Get_CXTranslationUnit_SkipFunctionBodies(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTranslationUnit_SkipFunctionBodies);
}

// Used to indicate that implicit attributes should be visited.
static Napi::Value Get_CXTranslationUnit_VisitImplicitAttributes(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTranslationUnit_VisitImplicitAttributes);
}

// Categorizes how memory is being used by a translation unit.
// enum CXTUResourceUsageKind
static Napi::Value Get_CXTUResourceUsage_AST(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTUResourceUsage_AST);
}

static Napi::Value Get_CXTUResourceUsage_AST_SideTables(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTUResourceUsage_AST_SideTables);
}

static Napi::Value Get_CXTUResourceUsage_ExternalASTSource_Membuffer_Malloc(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTUResourceUsage_ExternalASTSource_Membuffer_Malloc);
}

static Napi::Value Get_CXTUResourceUsage_ExternalASTSource_Membuffer_MMap(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTUResourceUsage_ExternalASTSource_Membuffer_MMap);
}

static Napi::Value Get_CXTUResourceUsage_First(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTUResourceUsage_First);
}

static Napi::Value Get_CXTUResourceUsage_GlobalCompletionResults(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTUResourceUsage_GlobalCompletionResults);
}

static Napi::Value Get_CXTUResourceUsage_Identifiers(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTUResourceUsage_Identifiers);
}

static Napi::Value Get_CXTUResourceUsage_Last(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTUResourceUsage_Last);
}

static Napi::Value Get_CXTUResourceUsage_MEMORY_IN_BYTES_BEGIN(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTUResourceUsage_MEMORY_IN_BYTES_BEGIN);
}

static Napi::Value Get_CXTUResourceUsage_MEMORY_IN_BYTES_END(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTUResourceUsage_MEMORY_IN_BYTES_END);
}

static Napi::Value Get_CXTUResourceUsage_PreprocessingRecord(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTUResourceUsage_PreprocessingRecord);
}

static Napi::Value Get_CXTUResourceUsage_Preprocessor(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTUResourceUsage_Preprocessor);
}

static Napi::Value Get_CXTUResourceUsage_Preprocessor_HeaderSearch(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTUResourceUsage_Preprocessor_HeaderSearch);
}

static Napi::Value Get_CXTUResourceUsage_Selectors(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTUResourceUsage_Selectors);
}

static Napi::Value Get_CXTUResourceUsage_SourceManager_DataStructures(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTUResourceUsage_SourceManager_DataStructures);
}

static Napi::Value Get_CXTUResourceUsage_SourceManager_Membuffer_Malloc(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTUResourceUsage_SourceManager_Membuffer_Malloc);
}

static Napi::Value Get_CXTUResourceUsage_SourceManager_Membuffer_MMap(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTUResourceUsage_SourceManager_Membuffer_MMap);
}

static Napi::Value Get_CXTUResourceUsage_SourceManagerContentCache(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTUResourceUsage_SourceManagerContentCache);
}

// Describes the kind of type
// enum CXTypeKind
// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Accum(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Accum);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_Atomic(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Atomic);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_Attributed(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Attributed);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Auto(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Auto);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_BFloat16(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_BFloat16);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_BlockPointer(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_BlockPointer);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Bool(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Bool);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_BTFTagAttributed(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_BTFTagAttributed);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Char_S(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Char_S);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Char_U(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Char_U);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Char16(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Char16);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Char32(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Char32);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Complex(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Complex);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_ConstantArray(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_ConstantArray);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Dependent(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Dependent);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_DependentSizedArray(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_DependentSizedArray);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Double(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Double);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_Elaborated(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Elaborated);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Enum(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Enum);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_ExtVector(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_ExtVector);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_FirstBuiltin(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_FirstBuiltin);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Float(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Float);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Float128(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Float128);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Float16(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Float16);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_FunctionNoProto(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_FunctionNoProto);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_FunctionProto(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_FunctionProto);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Half(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Half);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_HLSLAttributedResource(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_HLSLAttributedResource);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_HLSLResource(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_HLSLResource);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Ibm128(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Ibm128);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_IncompleteArray(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_IncompleteArray);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Int(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Int);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Int128(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Int128);
}

// Represents an invalid type (e.g., where no type is available).
static Napi::Value Get_CXType_Invalid(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Invalid);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_LastBuiltin(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_LastBuiltin);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Long(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Long);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_LongAccum(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_LongAccum);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_LongDouble(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_LongDouble);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_LongLong(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_LongLong);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_LValueReference(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_LValueReference);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_MemberPointer(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_MemberPointer);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_NullPtr(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_NullPtr);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_ObjCClass(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_ObjCClass);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_ObjCId(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_ObjCId);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_ObjCInterface(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_ObjCInterface);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_ObjCObject(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_ObjCObject);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_ObjCObjectPointer(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_ObjCObjectPointer);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_ObjCSel(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_ObjCSel);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_ObjCTypeParam(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_ObjCTypeParam);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLEvent(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLEvent);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage1dArrayRO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage1dArrayRO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage1dArrayRW(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage1dArrayRW);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage1dArrayWO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage1dArrayWO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage1dBufferRO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage1dBufferRO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage1dBufferRW(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage1dBufferRW);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage1dBufferWO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage1dBufferWO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage1dRO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage1dRO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage1dRW(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage1dRW);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage1dWO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage1dWO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dArrayDepthRO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dArrayDepthRO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dArrayDepthRW(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dArrayDepthRW);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dArrayDepthWO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dArrayDepthWO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dArrayMSAADepthRO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dArrayMSAADepthRO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dArrayMSAADepthRW(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dArrayMSAADepthRW);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dArrayMSAADepthWO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dArrayMSAADepthWO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dArrayMSAARO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dArrayMSAARO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dArrayMSAARW(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dArrayMSAARW);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dArrayMSAAWO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dArrayMSAAWO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dArrayRO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dArrayRO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dArrayRW(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dArrayRW);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dArrayWO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dArrayWO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dDepthRO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dDepthRO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dDepthRW(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dDepthRW);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dDepthWO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dDepthWO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dMSAADepthRO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dMSAADepthRO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dMSAADepthRW(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dMSAADepthRW);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dMSAADepthWO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dMSAADepthWO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dMSAARO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dMSAARO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dMSAARW(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dMSAARW);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dMSAAWO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dMSAAWO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dRO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dRO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dRW(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dRW);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage2dWO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage2dWO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage3dRO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage3dRO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage3dRW(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage3dRW);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLImage3dWO(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLImage3dWO);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLIntelSubgroupAVCImeDualReferenceStreamin(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLIntelSubgroupAVCImeDualReferenceStreamin);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLIntelSubgroupAVCImeDualRefStreamin(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLIntelSubgroupAVCImeDualRefStreamin);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLIntelSubgroupAVCImePayload(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLIntelSubgroupAVCImePayload);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLIntelSubgroupAVCImeResult(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLIntelSubgroupAVCImeResult);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLIntelSubgroupAVCImeResultDualReferenceStreamout(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLIntelSubgroupAVCImeResultDualReferenceStreamout);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLIntelSubgroupAVCImeResultDualRefStreamout(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLIntelSubgroupAVCImeResultDualRefStreamout);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLIntelSubgroupAVCImeResultSingleReferenceStreamout(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLIntelSubgroupAVCImeResultSingleReferenceStreamout);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLIntelSubgroupAVCImeResultSingleRefStreamout(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLIntelSubgroupAVCImeResultSingleRefStreamout);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLIntelSubgroupAVCImeSingleReferenceStreamin(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLIntelSubgroupAVCImeSingleReferenceStreamin);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLIntelSubgroupAVCImeSingleRefStreamin(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLIntelSubgroupAVCImeSingleRefStreamin);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLIntelSubgroupAVCMcePayload(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLIntelSubgroupAVCMcePayload);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLIntelSubgroupAVCMceResult(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLIntelSubgroupAVCMceResult);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLIntelSubgroupAVCRefPayload(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLIntelSubgroupAVCRefPayload);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLIntelSubgroupAVCRefResult(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLIntelSubgroupAVCRefResult);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLIntelSubgroupAVCSicPayload(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLIntelSubgroupAVCSicPayload);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLIntelSubgroupAVCSicResult(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLIntelSubgroupAVCSicResult);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLQueue(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLQueue);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLReserveID(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLReserveID);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_OCLSampler(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_OCLSampler);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Overload(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Overload);
}

// Represents a type that was referred to using an elaborated type keyword.
// 
// E.g., struct S, or via a qualified name, e.g., N::M::type, or both.
static Napi::Value Get_CXType_Pipe(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Pipe);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Pointer(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Pointer);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Record(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Record);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_RValueReference(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_RValueReference);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_SChar(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_SChar);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Short(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Short);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_ShortAccum(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_ShortAccum);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Typedef(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Typedef);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_UAccum(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_UAccum);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_UChar(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_UChar);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_UInt(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_UInt);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_UInt128(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_UInt128);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_ULong(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_ULong);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_ULongAccum(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_ULongAccum);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_ULongLong(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_ULongLong);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Unexposed(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Unexposed);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_UShort(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_UShort);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_UShortAccum(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_UShortAccum);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_VariableArray(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_VariableArray);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Vector(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Vector);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_Void(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_Void);
}

// A type whose specific kind is not exposed via this
// interface.
static Napi::Value Get_CXType_WChar(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXType_WChar);
}

// List the possible error codes for \c clang_Type_getSizeOf,
// \c clang_Type_getAlignOf, \c clang_Type_getOffsetOf,
// \c clang_Cursor_getOffsetOf, and \c clang_getOffsetOfBase.
// 
// A value of this enumeration type can be returned if the target type is not
// a valid argument to sizeof, alignof or offsetof.
// enum CXTypeLayoutError
// The type is a dependent Type.
static Napi::Value Get_CXTypeLayoutError_Dependent(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTypeLayoutError_Dependent);
}

// The type is an incomplete Type.
static Napi::Value Get_CXTypeLayoutError_Incomplete(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTypeLayoutError_Incomplete);
}

// Type is of kind CXType_Invalid.
static Napi::Value Get_CXTypeLayoutError_Invalid(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTypeLayoutError_Invalid);
}

// The Field name is not valid for this record.
static Napi::Value Get_CXTypeLayoutError_InvalidFieldName(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTypeLayoutError_InvalidFieldName);
}

// The type is not a constant size type.
static Napi::Value Get_CXTypeLayoutError_NotConstantSize(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTypeLayoutError_NotConstantSize);
}

// The type is undeduced.
static Napi::Value Get_CXTypeLayoutError_Undeduced(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTypeLayoutError_Undeduced);
}

// enum CXTypeNullabilityKind
// Nullability is not applicable to this type.
static Napi::Value Get_CXTypeNullability_Invalid(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTypeNullability_Invalid);
}

// Values of this type can never be null.
static Napi::Value Get_CXTypeNullability_NonNull(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTypeNullability_NonNull);
}

// Values of this type can be null.
static Napi::Value Get_CXTypeNullability_Nullable(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTypeNullability_Nullable);
}

// Generally behaves like Nullable, except when used in a block parameter that
// was imported into a swift async method. There, swift will assume that the
// parameter can get null even if no error occurred. _Nullable parameters are
// assumed to only get null on error.
static Napi::Value Get_CXTypeNullability_NullableResult(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTypeNullability_NullableResult);
}

// Whether values of this type can be null is (explicitly)
// unspecified. This captures a (fairly rare) case where we
// can't conclude anything about the nullability of the type even
// though it has been considered.
static Napi::Value Get_CXTypeNullability_Unspecified(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXTypeNullability_Unspecified);
}

// Describes the kind of unary operators.
// enum CXUnaryOperatorKind
// Address of operator.
static Napi::Value Get_CXUnaryOperator_AddrOf(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXUnaryOperator_AddrOf);
}

// C++ co_await operator.
static Napi::Value Get_CXUnaryOperator_Coawait(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXUnaryOperator_Coawait);
}

// Dereference operator.
static Napi::Value Get_CXUnaryOperator_Deref(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXUnaryOperator_Deref);
}

// __extension__ marker operator.
static Napi::Value Get_CXUnaryOperator_Extension(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXUnaryOperator_Extension);
}

// "__imag expr" operator.
static Napi::Value Get_CXUnaryOperator_Imag(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXUnaryOperator_Imag);
}

// This value describes cursors which are not unary operators.
static Napi::Value Get_CXUnaryOperator_Invalid(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXUnaryOperator_Invalid);
}

// LNot operator.
static Napi::Value Get_CXUnaryOperator_LNot(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXUnaryOperator_LNot);
}

// Minus operator.
static Napi::Value Get_CXUnaryOperator_Minus(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXUnaryOperator_Minus);
}

// Not operator.
static Napi::Value Get_CXUnaryOperator_Not(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXUnaryOperator_Not);
}

// Plus operator.
static Napi::Value Get_CXUnaryOperator_Plus(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXUnaryOperator_Plus);
}

// Postfix decrement operator.
static Napi::Value Get_CXUnaryOperator_PostDec(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXUnaryOperator_PostDec);
}

// Postfix increment operator.
static Napi::Value Get_CXUnaryOperator_PostInc(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXUnaryOperator_PostInc);
}

// Prefix decrement operator.
static Napi::Value Get_CXUnaryOperator_PreDec(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXUnaryOperator_PreDec);
}

// Prefix increment operator.
static Napi::Value Get_CXUnaryOperator_PreInc(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXUnaryOperator_PreInc);
}

// "__real expr" operator.
static Napi::Value Get_CXUnaryOperator_Real(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXUnaryOperator_Real);
}

// enum CXVisibilityKind
// Symbol seen by the linker and acts like a normal symbol.
static Napi::Value Get_CXVisibility_Default(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXVisibility_Default);
}

// Symbol not seen by the linker.
static Napi::Value Get_CXVisibility_Hidden(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXVisibility_Hidden);
}

// This value indicates that no visibility information is available
// for a provided CXCursor.
static Napi::Value Get_CXVisibility_Invalid(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXVisibility_Invalid);
}

// Symbol seen by the linker but resolves to a symbol inside this object.
static Napi::Value Get_CXVisibility_Protected(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXVisibility_Protected);
}

// \defgroup CINDEX_HIGH Higher level API functions
// 
// @{
// enum CXVisitorResult
static Napi::Value Get_CXVisit_Break(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXVisit_Break);
}

static Napi::Value Get_CXVisit_Continue(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), CXVisit_Continue);
}


// Struct wrappers
// Contains the results of code-completion.
// 
// This data structure contains the results of code completion, as
// produced by \c clang_codeCompleteAt(). Its contents must be freed by
// \c clang_disposeCodeCompleteResults.
static Napi::Value Create_CXCodeCompleteResults(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXCodeCompleteResults* ptr = new CXCodeCompleteResults();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("Results")) {
            if (obj.Get("Results").IsObject()) {
                ptr->Results = static_cast<CXCompletionResult *>(unwrapPointer(obj.Get("Results").As<Napi::Object>()));
            } else if (obj.Get("Results").IsNull() || obj.Get("Results").IsUndefined()) {
                ptr->Results = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->Results = nullptr;
            }
        }
        if (obj.Has("NumResults")) {
            ptr->NumResults = obj.Get("NumResults").As<Napi::Number>().Uint32Value();
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXCodeCompleteResults");
}

static Napi::Value Get_CXCodeCompleteResults_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXCodeCompleteResults* ptr = static_cast<CXCodeCompleteResults*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "NumResults") {
        return Napi::Number::New(env, ptr->NumResults);
    }
    if (fieldName == "Results") {
        return wrapPointer(env, ptr->Results, "CXCompletionResult");
    }
    
    return env.Undefined();
}

// A single result of code completion.
static Napi::Value Create_CXCompletionResult(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXCompletionResult* ptr = new CXCompletionResult();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("CursorKind")) {
            ptr->CursorKind = static_cast<enum CXCursorKind>(obj.Get("CursorKind").As<Napi::Number>().Int32Value());
        }
        if (obj.Has("CompletionString")) {
            ptr->CompletionString = obj.Get("CompletionString").As<Napi::External<void>>().Data();
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXCompletionResult");
}

static Napi::Value Get_CXCompletionResult_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXCompletionResult* ptr = static_cast<CXCompletionResult*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "CompletionString") {
        return wrapPointer(env, ptr->CompletionString, "CXCompletionString");
    }
    if (fieldName == "CursorKind") {
        return Napi::Number::New(env, static_cast<int>(ptr->CursorKind));
    }
    
    return env.Undefined();
}

// A cursor representing some element in the abstract syntax tree for
// a translation unit.
// 
// The cursor abstraction unifies the different kinds of entities in a
// program--declaration, statements, expressions, references to declarations,
// etc.--under a single "cursor" abstraction with a common set of operations.
// Common operation for a cursor include: getting the physical location in
// a source file where the cursor points, getting the name associated with a
// cursor, and retrieving cursors for any child nodes of a particular cursor.
// 
// Cursors can be produced in two specific ways.
// clang_getTranslationUnitCursor() produces a cursor for a translation unit,
// from which one can use clang_visitChildren() to explore the rest of the
// translation unit. clang_getCursor() maps from a physical source location
// to the entity that resides at that location, allowing one to map from the
// source code into the AST.
static Napi::Value Create_CXCursor(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXCursor* ptr = new CXCursor();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("kind")) {
            ptr->kind = static_cast<enum CXCursorKind>(obj.Get("kind").As<Napi::Number>().Int32Value());
        }
        if (obj.Has("xdata")) {
            ptr->xdata = obj.Get("xdata").As<Napi::Number>().Int32Value();
        }
        if (obj.Has("data")) {
            Napi::Array data_arr = obj.Get("data").As<Napi::Array>();
            for (size_t i = 0; i < data_arr.Length() && i < sizeof(ptr->data)/sizeof(ptr->data[0]); i++) {
                if (data_arr.Get(i).IsExternal()) {
                    ptr->data[i] = data_arr.Get(i).As<Napi::External<void>>().Data();
                } else {
                    ptr->data[i] = nullptr;
                }
            }
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXCursor");
}

static Napi::Value Get_CXCursor_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXCursor* ptr = static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "data") {
        Napi::Array arr = Napi::Array::New(env);
        size_t arraySize = sizeof(ptr->data)/sizeof(ptr->data[0]);
        for (size_t i = 0; i < arraySize; i++) {
            arr.Set(i, Napi::External<void>::New(env, const_cast<void*>(ptr->data[i])));
        }
        return arr;
    }
    if (fieldName == "kind") {
        return Napi::Number::New(env, static_cast<int>(ptr->kind));
    }
    if (fieldName == "xdata") {
        return Napi::Number::New(env, ptr->xdata);
    }
    
    return env.Undefined();
}

static Napi::Value Create_CXCursorAndRangeVisitor(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXCursorAndRangeVisitor* ptr = new CXCursorAndRangeVisitor();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("context")) {
            if (obj.Get("context").IsExternal()) {
                ptr->context = obj.Get("context").As<Napi::External<void>>().Data();
            } else {
                ptr->context = nullptr;
            }
        }
        if (obj.Has("visit")) {
            if (obj.Get("visit").IsExternal()) {
                ptr->visit = reinterpret_cast<enum CXVisitorResult (*)(void *, CXCursor, CXSourceRange)>(obj.Get("visit").As<Napi::External<void>>().Data());
            } else {
                ptr->visit = nullptr;
            }
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXCursorAndRangeVisitor");
}

static Napi::Value Get_CXCursorAndRangeVisitor_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXCursorAndRangeVisitor* ptr = static_cast<CXCursorAndRangeVisitor*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "context") {
        return wrapPointer(env, ptr->context, "void");
    }
    if (fieldName == "visit") {
        return Napi::External<void>::New(env, reinterpret_cast<void*>(ptr->visit));
    }
    
    return env.Undefined();
}

// Uniquely identifies a CXFile, that refers to the same underlying file,
// across an indexing session.
static Napi::Value Create_CXFileUniqueID(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXFileUniqueID* ptr = new CXFileUniqueID();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("data")) {
            Napi::Array data_arr = obj.Get("data").As<Napi::Array>();
            for (size_t i = 0; i < data_arr.Length() && i < sizeof(ptr->data)/sizeof(ptr->data[0]); i++) {
                ptr->data[i] = data_arr.Get(i).As<Napi::BigInt>().Uint64Value(nullptr);
            }
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXFileUniqueID");
}

static Napi::Value Get_CXFileUniqueID_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXFileUniqueID* ptr = static_cast<CXFileUniqueID*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "data") {
        Napi::Array arr = Napi::Array::New(env);
        size_t arraySize = sizeof(ptr->data)/sizeof(ptr->data[0]);
        for (size_t i = 0; i < arraySize; i++) {
            arr.Set(i, Napi::BigInt::New(env, ptr->data[i]));
        }
        return arr;
    }
    
    return env.Undefined();
}

static Napi::Value Create_CXIdxAttrInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXIdxAttrInfo* ptr = new CXIdxAttrInfo();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("kind")) {
            ptr->kind = static_cast<CXIdxAttrKind>(obj.Get("kind").As<Napi::Number>().Int32Value());
        }
        if (obj.Has("cursor")) {
            CXCursor* cursor_ptr = static_cast<CXCursor*>(unwrapPointer(obj.Get("cursor").As<Napi::Object>()));
            if (cursor_ptr) {
                ptr->cursor = *cursor_ptr;
            }
        }
        if (obj.Has("loc")) {
            CXIdxLoc* loc_ptr = static_cast<CXIdxLoc*>(unwrapPointer(obj.Get("loc").As<Napi::Object>()));
            if (loc_ptr) {
                ptr->loc = *loc_ptr;
            }
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXIdxAttrInfo");
}

static Napi::Value Get_CXIdxAttrInfo_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXIdxAttrInfo* ptr = static_cast<CXIdxAttrInfo*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "cursor") {
        CXCursor* fieldPtr = new CXCursor(ptr->cursor);
        return wrapPointer(env, fieldPtr, "CXCursor");
    }
    if (fieldName == "kind") {
        return Napi::Number::New(env, static_cast<int>(ptr->kind));
    }
    if (fieldName == "loc") {
        CXIdxLoc* fieldPtr = new CXIdxLoc(ptr->loc);
        return wrapPointer(env, fieldPtr, "CXIdxLoc");
    }
    
    return env.Undefined();
}

static Napi::Value Create_CXIdxBaseClassInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXIdxBaseClassInfo* ptr = new CXIdxBaseClassInfo();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("base")) {
            if (obj.Get("base").IsObject()) {
                ptr->base = static_cast<const CXIdxEntityInfo *>(unwrapPointer(obj.Get("base").As<Napi::Object>()));
            } else if (obj.Get("base").IsNull() || obj.Get("base").IsUndefined()) {
                ptr->base = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->base = nullptr;
            }
        }
        if (obj.Has("cursor")) {
            CXCursor* cursor_ptr = static_cast<CXCursor*>(unwrapPointer(obj.Get("cursor").As<Napi::Object>()));
            if (cursor_ptr) {
                ptr->cursor = *cursor_ptr;
            }
        }
        if (obj.Has("loc")) {
            CXIdxLoc* loc_ptr = static_cast<CXIdxLoc*>(unwrapPointer(obj.Get("loc").As<Napi::Object>()));
            if (loc_ptr) {
                ptr->loc = *loc_ptr;
            }
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXIdxBaseClassInfo");
}

static Napi::Value Get_CXIdxBaseClassInfo_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXIdxBaseClassInfo* ptr = static_cast<CXIdxBaseClassInfo*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "base") {
        return wrapConstPointer(env, ptr->base, "CXIdxEntityInfo");
    }
    if (fieldName == "cursor") {
        CXCursor* fieldPtr = new CXCursor(ptr->cursor);
        return wrapPointer(env, fieldPtr, "CXCursor");
    }
    if (fieldName == "loc") {
        CXIdxLoc* fieldPtr = new CXIdxLoc(ptr->loc);
        return wrapPointer(env, fieldPtr, "CXIdxLoc");
    }
    
    return env.Undefined();
}

static Napi::Value Create_CXIdxContainerInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXIdxContainerInfo* ptr = new CXIdxContainerInfo();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("cursor")) {
            CXCursor* cursor_ptr = static_cast<CXCursor*>(unwrapPointer(obj.Get("cursor").As<Napi::Object>()));
            if (cursor_ptr) {
                ptr->cursor = *cursor_ptr;
            }
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXIdxContainerInfo");
}

static Napi::Value Get_CXIdxContainerInfo_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXIdxContainerInfo* ptr = static_cast<CXIdxContainerInfo*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "cursor") {
        CXCursor* fieldPtr = new CXCursor(ptr->cursor);
        return wrapPointer(env, fieldPtr, "CXCursor");
    }
    
    return env.Undefined();
}

static Napi::Value Create_CXIdxCXXClassDeclInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXIdxCXXClassDeclInfo* ptr = new CXIdxCXXClassDeclInfo();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("declInfo")) {
            if (obj.Get("declInfo").IsObject()) {
                ptr->declInfo = static_cast<const CXIdxDeclInfo *>(unwrapPointer(obj.Get("declInfo").As<Napi::Object>()));
            } else if (obj.Get("declInfo").IsNull() || obj.Get("declInfo").IsUndefined()) {
                ptr->declInfo = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->declInfo = nullptr;
            }
        }
        if (obj.Has("bases")) {
            if (obj.Get("bases").IsObject()) {
                ptr->bases = static_cast<const CXIdxBaseClassInfo *const *>(unwrapPointer(obj.Get("bases").As<Napi::Object>()));
            } else if (obj.Get("bases").IsNull() || obj.Get("bases").IsUndefined()) {
                ptr->bases = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->bases = nullptr;
            }
        }
        if (obj.Has("numBases")) {
            ptr->numBases = obj.Get("numBases").As<Napi::Number>().Uint32Value();
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXIdxCXXClassDeclInfo");
}

static Napi::Value Get_CXIdxCXXClassDeclInfo_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXIdxCXXClassDeclInfo* ptr = static_cast<CXIdxCXXClassDeclInfo*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "bases") {
        return wrapConstPointer(env, ptr->bases, "CXIdxBaseClassInfo *const");
    }
    if (fieldName == "declInfo") {
        return wrapConstPointer(env, ptr->declInfo, "CXIdxDeclInfo");
    }
    if (fieldName == "numBases") {
        return Napi::Number::New(env, ptr->numBases);
    }
    
    return env.Undefined();
}

static Napi::Value Create_CXIdxDeclInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXIdxDeclInfo* ptr = new CXIdxDeclInfo();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("entityInfo")) {
            if (obj.Get("entityInfo").IsObject()) {
                ptr->entityInfo = static_cast<const CXIdxEntityInfo *>(unwrapPointer(obj.Get("entityInfo").As<Napi::Object>()));
            } else if (obj.Get("entityInfo").IsNull() || obj.Get("entityInfo").IsUndefined()) {
                ptr->entityInfo = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->entityInfo = nullptr;
            }
        }
        if (obj.Has("cursor")) {
            CXCursor* cursor_ptr = static_cast<CXCursor*>(unwrapPointer(obj.Get("cursor").As<Napi::Object>()));
            if (cursor_ptr) {
                ptr->cursor = *cursor_ptr;
            }
        }
        if (obj.Has("loc")) {
            CXIdxLoc* loc_ptr = static_cast<CXIdxLoc*>(unwrapPointer(obj.Get("loc").As<Napi::Object>()));
            if (loc_ptr) {
                ptr->loc = *loc_ptr;
            }
        }
        if (obj.Has("semanticContainer")) {
            if (obj.Get("semanticContainer").IsObject()) {
                ptr->semanticContainer = static_cast<const CXIdxContainerInfo *>(unwrapPointer(obj.Get("semanticContainer").As<Napi::Object>()));
            } else if (obj.Get("semanticContainer").IsNull() || obj.Get("semanticContainer").IsUndefined()) {
                ptr->semanticContainer = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->semanticContainer = nullptr;
            }
        }
        if (obj.Has("lexicalContainer")) {
            if (obj.Get("lexicalContainer").IsObject()) {
                ptr->lexicalContainer = static_cast<const CXIdxContainerInfo *>(unwrapPointer(obj.Get("lexicalContainer").As<Napi::Object>()));
            } else if (obj.Get("lexicalContainer").IsNull() || obj.Get("lexicalContainer").IsUndefined()) {
                ptr->lexicalContainer = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->lexicalContainer = nullptr;
            }
        }
        if (obj.Has("isRedeclaration")) {
            ptr->isRedeclaration = obj.Get("isRedeclaration").As<Napi::Number>().Int32Value();
        }
        if (obj.Has("isDefinition")) {
            ptr->isDefinition = obj.Get("isDefinition").As<Napi::Number>().Int32Value();
        }
        if (obj.Has("isContainer")) {
            ptr->isContainer = obj.Get("isContainer").As<Napi::Number>().Int32Value();
        }
        if (obj.Has("declAsContainer")) {
            if (obj.Get("declAsContainer").IsObject()) {
                ptr->declAsContainer = static_cast<const CXIdxContainerInfo *>(unwrapPointer(obj.Get("declAsContainer").As<Napi::Object>()));
            } else if (obj.Get("declAsContainer").IsNull() || obj.Get("declAsContainer").IsUndefined()) {
                ptr->declAsContainer = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->declAsContainer = nullptr;
            }
        }
        if (obj.Has("isImplicit")) {
            ptr->isImplicit = obj.Get("isImplicit").As<Napi::Number>().Int32Value();
        }
        if (obj.Has("attributes")) {
            if (obj.Get("attributes").IsObject()) {
                ptr->attributes = static_cast<const CXIdxAttrInfo *const *>(unwrapPointer(obj.Get("attributes").As<Napi::Object>()));
            } else if (obj.Get("attributes").IsNull() || obj.Get("attributes").IsUndefined()) {
                ptr->attributes = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->attributes = nullptr;
            }
        }
        if (obj.Has("numAttributes")) {
            ptr->numAttributes = obj.Get("numAttributes").As<Napi::Number>().Uint32Value();
        }
        if (obj.Has("flags")) {
            ptr->flags = obj.Get("flags").As<Napi::Number>().Uint32Value();
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXIdxDeclInfo");
}

static Napi::Value Get_CXIdxDeclInfo_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXIdxDeclInfo* ptr = static_cast<CXIdxDeclInfo*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "attributes") {
        return wrapConstPointer(env, ptr->attributes, "CXIdxAttrInfo *const");
    }
    if (fieldName == "cursor") {
        CXCursor* fieldPtr = new CXCursor(ptr->cursor);
        return wrapPointer(env, fieldPtr, "CXCursor");
    }
    if (fieldName == "declAsContainer") {
        return wrapConstPointer(env, ptr->declAsContainer, "CXIdxContainerInfo");
    }
    if (fieldName == "entityInfo") {
        return wrapConstPointer(env, ptr->entityInfo, "CXIdxEntityInfo");
    }
    if (fieldName == "flags") {
        return Napi::Number::New(env, ptr->flags);
    }
    if (fieldName == "isContainer") {
        return Napi::Number::New(env, ptr->isContainer);
    }
    if (fieldName == "isDefinition") {
        return Napi::Number::New(env, ptr->isDefinition);
    }
    if (fieldName == "isImplicit") {
        return Napi::Number::New(env, ptr->isImplicit);
    }
    if (fieldName == "isRedeclaration") {
        return Napi::Number::New(env, ptr->isRedeclaration);
    }
    if (fieldName == "lexicalContainer") {
        return wrapConstPointer(env, ptr->lexicalContainer, "CXIdxContainerInfo");
    }
    if (fieldName == "loc") {
        CXIdxLoc* fieldPtr = new CXIdxLoc(ptr->loc);
        return wrapPointer(env, fieldPtr, "CXIdxLoc");
    }
    if (fieldName == "numAttributes") {
        return Napi::Number::New(env, ptr->numAttributes);
    }
    if (fieldName == "semanticContainer") {
        return wrapConstPointer(env, ptr->semanticContainer, "CXIdxContainerInfo");
    }
    
    return env.Undefined();
}

static Napi::Value Create_CXIdxEntityInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXIdxEntityInfo* ptr = new CXIdxEntityInfo();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("kind")) {
            ptr->kind = static_cast<CXIdxEntityKind>(obj.Get("kind").As<Napi::Number>().Int32Value());
        }
        if (obj.Has("templateKind")) {
            ptr->templateKind = static_cast<CXIdxEntityCXXTemplateKind>(obj.Get("templateKind").As<Napi::Number>().Int32Value());
        }
        if (obj.Has("lang")) {
            ptr->lang = static_cast<CXIdxEntityLanguage>(obj.Get("lang").As<Napi::Number>().Int32Value());
        }
        if (obj.Has("name")) {
            // Note: String lifetime management needed for production use
            std::string name_str = obj.Get("name").As<Napi::String>().Utf8Value();
            ptr->name = name_str.c_str();
        }
        if (obj.Has("USR")) {
            // Note: String lifetime management needed for production use
            std::string USR_str = obj.Get("USR").As<Napi::String>().Utf8Value();
            ptr->USR = USR_str.c_str();
        }
        if (obj.Has("cursor")) {
            CXCursor* cursor_ptr = static_cast<CXCursor*>(unwrapPointer(obj.Get("cursor").As<Napi::Object>()));
            if (cursor_ptr) {
                ptr->cursor = *cursor_ptr;
            }
        }
        if (obj.Has("attributes")) {
            if (obj.Get("attributes").IsObject()) {
                ptr->attributes = static_cast<const CXIdxAttrInfo *const *>(unwrapPointer(obj.Get("attributes").As<Napi::Object>()));
            } else if (obj.Get("attributes").IsNull() || obj.Get("attributes").IsUndefined()) {
                ptr->attributes = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->attributes = nullptr;
            }
        }
        if (obj.Has("numAttributes")) {
            ptr->numAttributes = obj.Get("numAttributes").As<Napi::Number>().Uint32Value();
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXIdxEntityInfo");
}

static Napi::Value Get_CXIdxEntityInfo_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXIdxEntityInfo* ptr = static_cast<CXIdxEntityInfo*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "attributes") {
        return wrapConstPointer(env, ptr->attributes, "CXIdxAttrInfo *const");
    }
    if (fieldName == "cursor") {
        CXCursor* fieldPtr = new CXCursor(ptr->cursor);
        return wrapPointer(env, fieldPtr, "CXCursor");
    }
    if (fieldName == "kind") {
        return Napi::Number::New(env, static_cast<int>(ptr->kind));
    }
    if (fieldName == "lang") {
        return Napi::Number::New(env, static_cast<int>(ptr->lang));
    }
    if (fieldName == "name") {
        return Napi::String::New(env, ptr->name);
    }
    if (fieldName == "numAttributes") {
        return Napi::Number::New(env, ptr->numAttributes);
    }
    if (fieldName == "templateKind") {
        return Napi::Number::New(env, static_cast<int>(ptr->templateKind));
    }
    if (fieldName == "USR") {
        return Napi::String::New(env, ptr->USR);
    }
    
    return env.Undefined();
}

// Data for IndexerCallbacks#indexEntityReference.
static Napi::Value Create_CXIdxEntityRefInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXIdxEntityRefInfo* ptr = new CXIdxEntityRefInfo();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("kind")) {
            ptr->kind = static_cast<CXIdxEntityRefKind>(obj.Get("kind").As<Napi::Number>().Int32Value());
        }
        if (obj.Has("cursor")) {
            CXCursor* cursor_ptr = static_cast<CXCursor*>(unwrapPointer(obj.Get("cursor").As<Napi::Object>()));
            if (cursor_ptr) {
                ptr->cursor = *cursor_ptr;
            }
        }
        if (obj.Has("loc")) {
            CXIdxLoc* loc_ptr = static_cast<CXIdxLoc*>(unwrapPointer(obj.Get("loc").As<Napi::Object>()));
            if (loc_ptr) {
                ptr->loc = *loc_ptr;
            }
        }
        if (obj.Has("referencedEntity")) {
            if (obj.Get("referencedEntity").IsObject()) {
                ptr->referencedEntity = static_cast<const CXIdxEntityInfo *>(unwrapPointer(obj.Get("referencedEntity").As<Napi::Object>()));
            } else if (obj.Get("referencedEntity").IsNull() || obj.Get("referencedEntity").IsUndefined()) {
                ptr->referencedEntity = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->referencedEntity = nullptr;
            }
        }
        if (obj.Has("parentEntity")) {
            if (obj.Get("parentEntity").IsObject()) {
                ptr->parentEntity = static_cast<const CXIdxEntityInfo *>(unwrapPointer(obj.Get("parentEntity").As<Napi::Object>()));
            } else if (obj.Get("parentEntity").IsNull() || obj.Get("parentEntity").IsUndefined()) {
                ptr->parentEntity = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->parentEntity = nullptr;
            }
        }
        if (obj.Has("container")) {
            if (obj.Get("container").IsObject()) {
                ptr->container = static_cast<const CXIdxContainerInfo *>(unwrapPointer(obj.Get("container").As<Napi::Object>()));
            } else if (obj.Get("container").IsNull() || obj.Get("container").IsUndefined()) {
                ptr->container = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->container = nullptr;
            }
        }
        if (obj.Has("role")) {
            ptr->role = static_cast<CXSymbolRole>(obj.Get("role").As<Napi::Number>().Int32Value());
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXIdxEntityRefInfo");
}

static Napi::Value Get_CXIdxEntityRefInfo_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXIdxEntityRefInfo* ptr = static_cast<CXIdxEntityRefInfo*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "container") {
        return wrapConstPointer(env, ptr->container, "CXIdxContainerInfo");
    }
    if (fieldName == "cursor") {
        CXCursor* fieldPtr = new CXCursor(ptr->cursor);
        return wrapPointer(env, fieldPtr, "CXCursor");
    }
    if (fieldName == "kind") {
        return Napi::Number::New(env, static_cast<int>(ptr->kind));
    }
    if (fieldName == "loc") {
        CXIdxLoc* fieldPtr = new CXIdxLoc(ptr->loc);
        return wrapPointer(env, fieldPtr, "CXIdxLoc");
    }
    if (fieldName == "parentEntity") {
        return wrapConstPointer(env, ptr->parentEntity, "CXIdxEntityInfo");
    }
    if (fieldName == "referencedEntity") {
        return wrapConstPointer(env, ptr->referencedEntity, "CXIdxEntityInfo");
    }
    if (fieldName == "role") {
        return Napi::Number::New(env, static_cast<int>(ptr->role));
    }
    
    return env.Undefined();
}

static Napi::Value Create_CXIdxIBOutletCollectionAttrInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXIdxIBOutletCollectionAttrInfo* ptr = new CXIdxIBOutletCollectionAttrInfo();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("attrInfo")) {
            if (obj.Get("attrInfo").IsObject()) {
                ptr->attrInfo = static_cast<const CXIdxAttrInfo *>(unwrapPointer(obj.Get("attrInfo").As<Napi::Object>()));
            } else if (obj.Get("attrInfo").IsNull() || obj.Get("attrInfo").IsUndefined()) {
                ptr->attrInfo = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->attrInfo = nullptr;
            }
        }
        if (obj.Has("objcClass")) {
            if (obj.Get("objcClass").IsObject()) {
                ptr->objcClass = static_cast<const CXIdxEntityInfo *>(unwrapPointer(obj.Get("objcClass").As<Napi::Object>()));
            } else if (obj.Get("objcClass").IsNull() || obj.Get("objcClass").IsUndefined()) {
                ptr->objcClass = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->objcClass = nullptr;
            }
        }
        if (obj.Has("classCursor")) {
            CXCursor* classCursor_ptr = static_cast<CXCursor*>(unwrapPointer(obj.Get("classCursor").As<Napi::Object>()));
            if (classCursor_ptr) {
                ptr->classCursor = *classCursor_ptr;
            }
        }
        if (obj.Has("classLoc")) {
            CXIdxLoc* classLoc_ptr = static_cast<CXIdxLoc*>(unwrapPointer(obj.Get("classLoc").As<Napi::Object>()));
            if (classLoc_ptr) {
                ptr->classLoc = *classLoc_ptr;
            }
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXIdxIBOutletCollectionAttrInfo");
}

static Napi::Value Get_CXIdxIBOutletCollectionAttrInfo_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXIdxIBOutletCollectionAttrInfo* ptr = static_cast<CXIdxIBOutletCollectionAttrInfo*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "attrInfo") {
        return wrapConstPointer(env, ptr->attrInfo, "CXIdxAttrInfo");
    }
    if (fieldName == "classCursor") {
        CXCursor* fieldPtr = new CXCursor(ptr->classCursor);
        return wrapPointer(env, fieldPtr, "CXCursor");
    }
    if (fieldName == "classLoc") {
        CXIdxLoc* fieldPtr = new CXIdxLoc(ptr->classLoc);
        return wrapPointer(env, fieldPtr, "CXIdxLoc");
    }
    if (fieldName == "objcClass") {
        return wrapConstPointer(env, ptr->objcClass, "CXIdxEntityInfo");
    }
    
    return env.Undefined();
}

// Data for IndexerCallbacks#importedASTFile.
static Napi::Value Create_CXIdxImportedASTFileInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXIdxImportedASTFileInfo* ptr = new CXIdxImportedASTFileInfo();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("file")) {
            ptr->file = obj.Get("file").As<Napi::External<void>>().Data();
        }
        if (obj.Has("module")) {
            ptr->module = obj.Get("module").As<Napi::External<void>>().Data();
        }
        if (obj.Has("loc")) {
            CXIdxLoc* loc_ptr = static_cast<CXIdxLoc*>(unwrapPointer(obj.Get("loc").As<Napi::Object>()));
            if (loc_ptr) {
                ptr->loc = *loc_ptr;
            }
        }
        if (obj.Has("isImplicit")) {
            ptr->isImplicit = obj.Get("isImplicit").As<Napi::Number>().Int32Value();
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXIdxImportedASTFileInfo");
}

static Napi::Value Get_CXIdxImportedASTFileInfo_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXIdxImportedASTFileInfo* ptr = static_cast<CXIdxImportedASTFileInfo*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "file") {
        return wrapPointer(env, ptr->file, "CXFile");
    }
    if (fieldName == "isImplicit") {
        return Napi::Number::New(env, ptr->isImplicit);
    }
    if (fieldName == "loc") {
        CXIdxLoc* fieldPtr = new CXIdxLoc(ptr->loc);
        return wrapPointer(env, fieldPtr, "CXIdxLoc");
    }
    if (fieldName == "module") {
        return wrapPointer(env, ptr->module, "CXModule");
    }
    
    return env.Undefined();
}

// Data for ppIncludedFile callback.
static Napi::Value Create_CXIdxIncludedFileInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXIdxIncludedFileInfo* ptr = new CXIdxIncludedFileInfo();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("hashLoc")) {
            CXIdxLoc* hashLoc_ptr = static_cast<CXIdxLoc*>(unwrapPointer(obj.Get("hashLoc").As<Napi::Object>()));
            if (hashLoc_ptr) {
                ptr->hashLoc = *hashLoc_ptr;
            }
        }
        if (obj.Has("filename")) {
            // Note: String lifetime management needed for production use
            std::string filename_str = obj.Get("filename").As<Napi::String>().Utf8Value();
            ptr->filename = filename_str.c_str();
        }
        if (obj.Has("file")) {
            ptr->file = obj.Get("file").As<Napi::External<void>>().Data();
        }
        if (obj.Has("isImport")) {
            ptr->isImport = obj.Get("isImport").As<Napi::Number>().Int32Value();
        }
        if (obj.Has("isAngled")) {
            ptr->isAngled = obj.Get("isAngled").As<Napi::Number>().Int32Value();
        }
        if (obj.Has("isModuleImport")) {
            ptr->isModuleImport = obj.Get("isModuleImport").As<Napi::Number>().Int32Value();
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXIdxIncludedFileInfo");
}

static Napi::Value Get_CXIdxIncludedFileInfo_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXIdxIncludedFileInfo* ptr = static_cast<CXIdxIncludedFileInfo*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "file") {
        return wrapPointer(env, ptr->file, "CXFile");
    }
    if (fieldName == "filename") {
        return Napi::String::New(env, ptr->filename);
    }
    if (fieldName == "hashLoc") {
        CXIdxLoc* fieldPtr = new CXIdxLoc(ptr->hashLoc);
        return wrapPointer(env, fieldPtr, "CXIdxLoc");
    }
    if (fieldName == "isAngled") {
        return Napi::Number::New(env, ptr->isAngled);
    }
    if (fieldName == "isImport") {
        return Napi::Number::New(env, ptr->isImport);
    }
    if (fieldName == "isModuleImport") {
        return Napi::Number::New(env, ptr->isModuleImport);
    }
    
    return env.Undefined();
}

// Source location passed to index callbacks.
static Napi::Value Create_CXIdxLoc(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXIdxLoc* ptr = new CXIdxLoc();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("ptr_data")) {
            Napi::Array ptr_data_arr = obj.Get("ptr_data").As<Napi::Array>();
            for (size_t i = 0; i < ptr_data_arr.Length() && i < sizeof(ptr->ptr_data)/sizeof(ptr->ptr_data[0]); i++) {
                if (ptr_data_arr.Get(i).IsExternal()) {
                    ptr->ptr_data[i] = ptr_data_arr.Get(i).As<Napi::External<void>>().Data();
                } else {
                    ptr->ptr_data[i] = nullptr;
                }
            }
        }
        if (obj.Has("int_data")) {
            ptr->int_data = obj.Get("int_data").As<Napi::Number>().Uint32Value();
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXIdxLoc");
}

static Napi::Value Get_CXIdxLoc_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXIdxLoc* ptr = static_cast<CXIdxLoc*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "int_data") {
        return Napi::Number::New(env, ptr->int_data);
    }
    if (fieldName == "ptr_data") {
        Napi::Array arr = Napi::Array::New(env);
        size_t arraySize = sizeof(ptr->ptr_data)/sizeof(ptr->ptr_data[0]);
        for (size_t i = 0; i < arraySize; i++) {
            arr.Set(i, Napi::External<void>::New(env, const_cast<void*>(ptr->ptr_data[i])));
        }
        return arr;
    }
    
    return env.Undefined();
}

static Napi::Value Create_CXIdxObjCCategoryDeclInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXIdxObjCCategoryDeclInfo* ptr = new CXIdxObjCCategoryDeclInfo();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("containerInfo")) {
            if (obj.Get("containerInfo").IsObject()) {
                ptr->containerInfo = static_cast<const CXIdxObjCContainerDeclInfo *>(unwrapPointer(obj.Get("containerInfo").As<Napi::Object>()));
            } else if (obj.Get("containerInfo").IsNull() || obj.Get("containerInfo").IsUndefined()) {
                ptr->containerInfo = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->containerInfo = nullptr;
            }
        }
        if (obj.Has("objcClass")) {
            if (obj.Get("objcClass").IsObject()) {
                ptr->objcClass = static_cast<const CXIdxEntityInfo *>(unwrapPointer(obj.Get("objcClass").As<Napi::Object>()));
            } else if (obj.Get("objcClass").IsNull() || obj.Get("objcClass").IsUndefined()) {
                ptr->objcClass = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->objcClass = nullptr;
            }
        }
        if (obj.Has("classCursor")) {
            CXCursor* classCursor_ptr = static_cast<CXCursor*>(unwrapPointer(obj.Get("classCursor").As<Napi::Object>()));
            if (classCursor_ptr) {
                ptr->classCursor = *classCursor_ptr;
            }
        }
        if (obj.Has("classLoc")) {
            CXIdxLoc* classLoc_ptr = static_cast<CXIdxLoc*>(unwrapPointer(obj.Get("classLoc").As<Napi::Object>()));
            if (classLoc_ptr) {
                ptr->classLoc = *classLoc_ptr;
            }
        }
        if (obj.Has("protocols")) {
            if (obj.Get("protocols").IsObject()) {
                ptr->protocols = static_cast<const CXIdxObjCProtocolRefListInfo *>(unwrapPointer(obj.Get("protocols").As<Napi::Object>()));
            } else if (obj.Get("protocols").IsNull() || obj.Get("protocols").IsUndefined()) {
                ptr->protocols = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->protocols = nullptr;
            }
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXIdxObjCCategoryDeclInfo");
}

static Napi::Value Get_CXIdxObjCCategoryDeclInfo_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXIdxObjCCategoryDeclInfo* ptr = static_cast<CXIdxObjCCategoryDeclInfo*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "classCursor") {
        CXCursor* fieldPtr = new CXCursor(ptr->classCursor);
        return wrapPointer(env, fieldPtr, "CXCursor");
    }
    if (fieldName == "classLoc") {
        CXIdxLoc* fieldPtr = new CXIdxLoc(ptr->classLoc);
        return wrapPointer(env, fieldPtr, "CXIdxLoc");
    }
    if (fieldName == "containerInfo") {
        return wrapConstPointer(env, ptr->containerInfo, "CXIdxObjCContainerDeclInfo");
    }
    if (fieldName == "objcClass") {
        return wrapConstPointer(env, ptr->objcClass, "CXIdxEntityInfo");
    }
    if (fieldName == "protocols") {
        return wrapConstPointer(env, ptr->protocols, "CXIdxObjCProtocolRefListInfo");
    }
    
    return env.Undefined();
}

static Napi::Value Create_CXIdxObjCContainerDeclInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXIdxObjCContainerDeclInfo* ptr = new CXIdxObjCContainerDeclInfo();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("declInfo")) {
            if (obj.Get("declInfo").IsObject()) {
                ptr->declInfo = static_cast<const CXIdxDeclInfo *>(unwrapPointer(obj.Get("declInfo").As<Napi::Object>()));
            } else if (obj.Get("declInfo").IsNull() || obj.Get("declInfo").IsUndefined()) {
                ptr->declInfo = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->declInfo = nullptr;
            }
        }
        if (obj.Has("kind")) {
            ptr->kind = static_cast<CXIdxObjCContainerKind>(obj.Get("kind").As<Napi::Number>().Int32Value());
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXIdxObjCContainerDeclInfo");
}

static Napi::Value Get_CXIdxObjCContainerDeclInfo_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXIdxObjCContainerDeclInfo* ptr = static_cast<CXIdxObjCContainerDeclInfo*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "declInfo") {
        return wrapConstPointer(env, ptr->declInfo, "CXIdxDeclInfo");
    }
    if (fieldName == "kind") {
        return Napi::Number::New(env, static_cast<int>(ptr->kind));
    }
    
    return env.Undefined();
}

static Napi::Value Create_CXIdxObjCInterfaceDeclInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXIdxObjCInterfaceDeclInfo* ptr = new CXIdxObjCInterfaceDeclInfo();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("containerInfo")) {
            if (obj.Get("containerInfo").IsObject()) {
                ptr->containerInfo = static_cast<const CXIdxObjCContainerDeclInfo *>(unwrapPointer(obj.Get("containerInfo").As<Napi::Object>()));
            } else if (obj.Get("containerInfo").IsNull() || obj.Get("containerInfo").IsUndefined()) {
                ptr->containerInfo = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->containerInfo = nullptr;
            }
        }
        if (obj.Has("superInfo")) {
            if (obj.Get("superInfo").IsObject()) {
                ptr->superInfo = static_cast<const CXIdxBaseClassInfo *>(unwrapPointer(obj.Get("superInfo").As<Napi::Object>()));
            } else if (obj.Get("superInfo").IsNull() || obj.Get("superInfo").IsUndefined()) {
                ptr->superInfo = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->superInfo = nullptr;
            }
        }
        if (obj.Has("protocols")) {
            if (obj.Get("protocols").IsObject()) {
                ptr->protocols = static_cast<const CXIdxObjCProtocolRefListInfo *>(unwrapPointer(obj.Get("protocols").As<Napi::Object>()));
            } else if (obj.Get("protocols").IsNull() || obj.Get("protocols").IsUndefined()) {
                ptr->protocols = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->protocols = nullptr;
            }
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXIdxObjCInterfaceDeclInfo");
}

static Napi::Value Get_CXIdxObjCInterfaceDeclInfo_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXIdxObjCInterfaceDeclInfo* ptr = static_cast<CXIdxObjCInterfaceDeclInfo*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "containerInfo") {
        return wrapConstPointer(env, ptr->containerInfo, "CXIdxObjCContainerDeclInfo");
    }
    if (fieldName == "protocols") {
        return wrapConstPointer(env, ptr->protocols, "CXIdxObjCProtocolRefListInfo");
    }
    if (fieldName == "superInfo") {
        return wrapConstPointer(env, ptr->superInfo, "CXIdxBaseClassInfo");
    }
    
    return env.Undefined();
}

static Napi::Value Create_CXIdxObjCPropertyDeclInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXIdxObjCPropertyDeclInfo* ptr = new CXIdxObjCPropertyDeclInfo();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("declInfo")) {
            if (obj.Get("declInfo").IsObject()) {
                ptr->declInfo = static_cast<const CXIdxDeclInfo *>(unwrapPointer(obj.Get("declInfo").As<Napi::Object>()));
            } else if (obj.Get("declInfo").IsNull() || obj.Get("declInfo").IsUndefined()) {
                ptr->declInfo = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->declInfo = nullptr;
            }
        }
        if (obj.Has("getter")) {
            if (obj.Get("getter").IsObject()) {
                ptr->getter = static_cast<const CXIdxEntityInfo *>(unwrapPointer(obj.Get("getter").As<Napi::Object>()));
            } else if (obj.Get("getter").IsNull() || obj.Get("getter").IsUndefined()) {
                ptr->getter = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->getter = nullptr;
            }
        }
        if (obj.Has("setter")) {
            if (obj.Get("setter").IsObject()) {
                ptr->setter = static_cast<const CXIdxEntityInfo *>(unwrapPointer(obj.Get("setter").As<Napi::Object>()));
            } else if (obj.Get("setter").IsNull() || obj.Get("setter").IsUndefined()) {
                ptr->setter = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->setter = nullptr;
            }
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXIdxObjCPropertyDeclInfo");
}

static Napi::Value Get_CXIdxObjCPropertyDeclInfo_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXIdxObjCPropertyDeclInfo* ptr = static_cast<CXIdxObjCPropertyDeclInfo*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "declInfo") {
        return wrapConstPointer(env, ptr->declInfo, "CXIdxDeclInfo");
    }
    if (fieldName == "getter") {
        return wrapConstPointer(env, ptr->getter, "CXIdxEntityInfo");
    }
    if (fieldName == "setter") {
        return wrapConstPointer(env, ptr->setter, "CXIdxEntityInfo");
    }
    
    return env.Undefined();
}

static Napi::Value Create_CXIdxObjCProtocolRefInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXIdxObjCProtocolRefInfo* ptr = new CXIdxObjCProtocolRefInfo();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("protocol")) {
            if (obj.Get("protocol").IsObject()) {
                ptr->protocol = static_cast<const CXIdxEntityInfo *>(unwrapPointer(obj.Get("protocol").As<Napi::Object>()));
            } else if (obj.Get("protocol").IsNull() || obj.Get("protocol").IsUndefined()) {
                ptr->protocol = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->protocol = nullptr;
            }
        }
        if (obj.Has("cursor")) {
            CXCursor* cursor_ptr = static_cast<CXCursor*>(unwrapPointer(obj.Get("cursor").As<Napi::Object>()));
            if (cursor_ptr) {
                ptr->cursor = *cursor_ptr;
            }
        }
        if (obj.Has("loc")) {
            CXIdxLoc* loc_ptr = static_cast<CXIdxLoc*>(unwrapPointer(obj.Get("loc").As<Napi::Object>()));
            if (loc_ptr) {
                ptr->loc = *loc_ptr;
            }
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXIdxObjCProtocolRefInfo");
}

static Napi::Value Get_CXIdxObjCProtocolRefInfo_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXIdxObjCProtocolRefInfo* ptr = static_cast<CXIdxObjCProtocolRefInfo*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "cursor") {
        CXCursor* fieldPtr = new CXCursor(ptr->cursor);
        return wrapPointer(env, fieldPtr, "CXCursor");
    }
    if (fieldName == "loc") {
        CXIdxLoc* fieldPtr = new CXIdxLoc(ptr->loc);
        return wrapPointer(env, fieldPtr, "CXIdxLoc");
    }
    if (fieldName == "protocol") {
        return wrapConstPointer(env, ptr->protocol, "CXIdxEntityInfo");
    }
    
    return env.Undefined();
}

static Napi::Value Create_CXIdxObjCProtocolRefListInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXIdxObjCProtocolRefListInfo* ptr = new CXIdxObjCProtocolRefListInfo();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("protocols")) {
            if (obj.Get("protocols").IsObject()) {
                ptr->protocols = static_cast<const CXIdxObjCProtocolRefInfo *const *>(unwrapPointer(obj.Get("protocols").As<Napi::Object>()));
            } else if (obj.Get("protocols").IsNull() || obj.Get("protocols").IsUndefined()) {
                ptr->protocols = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->protocols = nullptr;
            }
        }
        if (obj.Has("numProtocols")) {
            ptr->numProtocols = obj.Get("numProtocols").As<Napi::Number>().Uint32Value();
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXIdxObjCProtocolRefListInfo");
}

static Napi::Value Get_CXIdxObjCProtocolRefListInfo_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXIdxObjCProtocolRefListInfo* ptr = static_cast<CXIdxObjCProtocolRefListInfo*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "numProtocols") {
        return Napi::Number::New(env, ptr->numProtocols);
    }
    if (fieldName == "protocols") {
        return wrapConstPointer(env, ptr->protocols, "CXIdxObjCProtocolRefInfo *const");
    }
    
    return env.Undefined();
}

// Index initialization options.
// 
// 0 is the default value of each member of this struct except for Size.
// Initialize the struct in one of the following three ways to avoid adapting
// code each time a new member is added to it:
// \code
// CXIndexOptions Opts;
// memset(&Opts, 0, sizeof(Opts));
// Opts.Size = sizeof(CXIndexOptions);
// \endcode
// or explicitly initialize the first data member and zero-initialize the rest:
// \code
// CXIndexOptions Opts = { sizeof(CXIndexOptions) };
// \endcode
// or to prevent the -Wmissing-field-initializers warning for the above version:
// \code
// CXIndexOptions Opts{};
// Opts.Size = sizeof(CXIndexOptions);
// \endcode
static Napi::Value Create_CXIndexOptions(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXIndexOptions* ptr = new CXIndexOptions();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("Size")) {
            ptr->Size = obj.Get("Size").As<Napi::Number>().Uint32Value();
        }
        if (obj.Has("ThreadBackgroundPriorityForIndexing")) {
            ptr->ThreadBackgroundPriorityForIndexing = obj.Get("ThreadBackgroundPriorityForIndexing").As<Napi::Number>().Uint32Value();
        }
        if (obj.Has("ThreadBackgroundPriorityForEditing")) {
            ptr->ThreadBackgroundPriorityForEditing = obj.Get("ThreadBackgroundPriorityForEditing").As<Napi::Number>().Uint32Value();
        }
        if (obj.Has("ExcludeDeclarationsFromPCH")) {
            ptr->ExcludeDeclarationsFromPCH = obj.Get("ExcludeDeclarationsFromPCH").As<Napi::Number>().Uint32Value();
        }
        if (obj.Has("DisplayDiagnostics")) {
            ptr->DisplayDiagnostics = obj.Get("DisplayDiagnostics").As<Napi::Number>().Uint32Value();
        }
        if (obj.Has("StorePreamblesInMemory")) {
            ptr->StorePreamblesInMemory = obj.Get("StorePreamblesInMemory").As<Napi::Number>().Uint32Value();
        }
        if (obj.Has("PreambleStoragePath")) {
            // Note: String lifetime management needed for production use
            std::string PreambleStoragePath_str = obj.Get("PreambleStoragePath").As<Napi::String>().Utf8Value();
            ptr->PreambleStoragePath = PreambleStoragePath_str.c_str();
        }
        if (obj.Has("InvocationEmissionPath")) {
            // Note: String lifetime management needed for production use
            std::string InvocationEmissionPath_str = obj.Get("InvocationEmissionPath").As<Napi::String>().Utf8Value();
            ptr->InvocationEmissionPath = InvocationEmissionPath_str.c_str();
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXIndexOptions");
}

static Napi::Value Get_CXIndexOptions_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXIndexOptions* ptr = static_cast<CXIndexOptions*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "DisplayDiagnostics") {
        return Napi::Number::New(env, ptr->DisplayDiagnostics);
    }
    if (fieldName == "ExcludeDeclarationsFromPCH") {
        return Napi::Number::New(env, ptr->ExcludeDeclarationsFromPCH);
    }
    if (fieldName == "InvocationEmissionPath") {
        return Napi::String::New(env, ptr->InvocationEmissionPath);
    }
    if (fieldName == "PreambleStoragePath") {
        return Napi::String::New(env, ptr->PreambleStoragePath);
    }
    if (fieldName == "Size") {
        return Napi::Number::New(env, ptr->Size);
    }
    if (fieldName == "StorePreamblesInMemory") {
        return Napi::Number::New(env, ptr->StorePreamblesInMemory);
    }
    if (fieldName == "ThreadBackgroundPriorityForEditing") {
        return Napi::Number::New(env, ptr->ThreadBackgroundPriorityForEditing);
    }
    if (fieldName == "ThreadBackgroundPriorityForIndexing") {
        return Napi::Number::New(env, ptr->ThreadBackgroundPriorityForIndexing);
    }
    
    return env.Undefined();
}

// Describes the availability of a given entity on a particular platform, e.g.,
// a particular class might only be available on Mac OS 10.7 or newer.
static Napi::Value Create_CXPlatformAvailability(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXPlatformAvailability* ptr = new CXPlatformAvailability();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("Platform")) {
            CXString* Platform_ptr = static_cast<CXString*>(unwrapPointer(obj.Get("Platform").As<Napi::Object>()));
            if (Platform_ptr) {
                ptr->Platform = *Platform_ptr;
            }
        }
        if (obj.Has("Introduced")) {
            CXVersion* Introduced_ptr = static_cast<CXVersion*>(unwrapPointer(obj.Get("Introduced").As<Napi::Object>()));
            if (Introduced_ptr) {
                ptr->Introduced = *Introduced_ptr;
            }
        }
        if (obj.Has("Deprecated")) {
            CXVersion* Deprecated_ptr = static_cast<CXVersion*>(unwrapPointer(obj.Get("Deprecated").As<Napi::Object>()));
            if (Deprecated_ptr) {
                ptr->Deprecated = *Deprecated_ptr;
            }
        }
        if (obj.Has("Obsoleted")) {
            CXVersion* Obsoleted_ptr = static_cast<CXVersion*>(unwrapPointer(obj.Get("Obsoleted").As<Napi::Object>()));
            if (Obsoleted_ptr) {
                ptr->Obsoleted = *Obsoleted_ptr;
            }
        }
        if (obj.Has("Unavailable")) {
            ptr->Unavailable = obj.Get("Unavailable").As<Napi::Number>().Int32Value();
        }
        if (obj.Has("Message")) {
            CXString* Message_ptr = static_cast<CXString*>(unwrapPointer(obj.Get("Message").As<Napi::Object>()));
            if (Message_ptr) {
                ptr->Message = *Message_ptr;
            }
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXPlatformAvailability");
}

static Napi::Value Get_CXPlatformAvailability_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXPlatformAvailability* ptr = static_cast<CXPlatformAvailability*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "Deprecated") {
        CXVersion* fieldPtr = new CXVersion(ptr->Deprecated);
        return wrapPointer(env, fieldPtr, "CXVersion");
    }
    if (fieldName == "Introduced") {
        CXVersion* fieldPtr = new CXVersion(ptr->Introduced);
        return wrapPointer(env, fieldPtr, "CXVersion");
    }
    if (fieldName == "Message") {
        CXString* fieldPtr = new CXString(ptr->Message);
        return wrapPointer(env, fieldPtr, "CXString");
    }
    if (fieldName == "Obsoleted") {
        CXVersion* fieldPtr = new CXVersion(ptr->Obsoleted);
        return wrapPointer(env, fieldPtr, "CXVersion");
    }
    if (fieldName == "Platform") {
        CXString* fieldPtr = new CXString(ptr->Platform);
        return wrapPointer(env, fieldPtr, "CXString");
    }
    if (fieldName == "Unavailable") {
        return Napi::Number::New(env, ptr->Unavailable);
    }
    
    return env.Undefined();
}

// Identifies a specific source location within a translation
// unit.
// 
// Use clang_getExpansionLocation() or clang_getSpellingLocation()
// to map a source location to a particular file, line, and column.
static Napi::Value Create_CXSourceLocation(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXSourceLocation* ptr = new CXSourceLocation();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("ptr_data")) {
            Napi::Array ptr_data_arr = obj.Get("ptr_data").As<Napi::Array>();
            for (size_t i = 0; i < ptr_data_arr.Length() && i < sizeof(ptr->ptr_data)/sizeof(ptr->ptr_data[0]); i++) {
                if (ptr_data_arr.Get(i).IsExternal()) {
                    ptr->ptr_data[i] = ptr_data_arr.Get(i).As<Napi::External<void>>().Data();
                } else {
                    ptr->ptr_data[i] = nullptr;
                }
            }
        }
        if (obj.Has("int_data")) {
            ptr->int_data = obj.Get("int_data").As<Napi::Number>().Uint32Value();
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXSourceLocation");
}

static Napi::Value Get_CXSourceLocation_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXSourceLocation* ptr = static_cast<CXSourceLocation*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "int_data") {
        return Napi::Number::New(env, ptr->int_data);
    }
    if (fieldName == "ptr_data") {
        Napi::Array arr = Napi::Array::New(env);
        size_t arraySize = sizeof(ptr->ptr_data)/sizeof(ptr->ptr_data[0]);
        for (size_t i = 0; i < arraySize; i++) {
            arr.Set(i, Napi::External<void>::New(env, const_cast<void*>(ptr->ptr_data[i])));
        }
        return arr;
    }
    
    return env.Undefined();
}

// Identifies a half-open character range in the source code.
// 
// Use clang_getRangeStart() and clang_getRangeEnd() to retrieve the
// starting and end locations from a source range, respectively.
static Napi::Value Create_CXSourceRange(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXSourceRange* ptr = new CXSourceRange();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("ptr_data")) {
            Napi::Array ptr_data_arr = obj.Get("ptr_data").As<Napi::Array>();
            for (size_t i = 0; i < ptr_data_arr.Length() && i < sizeof(ptr->ptr_data)/sizeof(ptr->ptr_data[0]); i++) {
                if (ptr_data_arr.Get(i).IsExternal()) {
                    ptr->ptr_data[i] = ptr_data_arr.Get(i).As<Napi::External<void>>().Data();
                } else {
                    ptr->ptr_data[i] = nullptr;
                }
            }
        }
        if (obj.Has("begin_int_data")) {
            ptr->begin_int_data = obj.Get("begin_int_data").As<Napi::Number>().Uint32Value();
        }
        if (obj.Has("end_int_data")) {
            ptr->end_int_data = obj.Get("end_int_data").As<Napi::Number>().Uint32Value();
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXSourceRange");
}

static Napi::Value Get_CXSourceRange_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXSourceRange* ptr = static_cast<CXSourceRange*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "begin_int_data") {
        return Napi::Number::New(env, ptr->begin_int_data);
    }
    if (fieldName == "end_int_data") {
        return Napi::Number::New(env, ptr->end_int_data);
    }
    if (fieldName == "ptr_data") {
        Napi::Array arr = Napi::Array::New(env);
        size_t arraySize = sizeof(ptr->ptr_data)/sizeof(ptr->ptr_data[0]);
        for (size_t i = 0; i < arraySize; i++) {
            arr.Set(i, Napi::External<void>::New(env, const_cast<void*>(ptr->ptr_data[i])));
        }
        return arr;
    }
    
    return env.Undefined();
}

// Identifies an array of ranges.
static Napi::Value Create_CXSourceRangeList(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXSourceRangeList* ptr = new CXSourceRangeList();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("count")) {
            ptr->count = obj.Get("count").As<Napi::Number>().Uint32Value();
        }
        if (obj.Has("ranges")) {
            if (obj.Get("ranges").IsObject()) {
                ptr->ranges = static_cast<CXSourceRange *>(unwrapPointer(obj.Get("ranges").As<Napi::Object>()));
            } else if (obj.Get("ranges").IsNull() || obj.Get("ranges").IsUndefined()) {
                ptr->ranges = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->ranges = nullptr;
            }
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXSourceRangeList");
}

static Napi::Value Get_CXSourceRangeList_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXSourceRangeList* ptr = static_cast<CXSourceRangeList*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "count") {
        return Napi::Number::New(env, ptr->count);
    }
    if (fieldName == "ranges") {
        return wrapPointer(env, ptr->ranges, "CXSourceRange");
    }
    
    return env.Undefined();
}

// A character string.
// 
// The \c CXString type is used to return strings from the interface when
// the ownership of that string might differ from one call to the next.
// Use \c clang_getCString() to retrieve the string data and, once finished
// with the string data, call \c clang_disposeString() to free the string.
static Napi::Value Create_CXString(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXString* ptr = new CXString();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("data")) {
            if (obj.Get("data").IsObject()) {
                ptr->data = static_cast<const void *>(unwrapPointer(obj.Get("data").As<Napi::Object>()));
            } else if (obj.Get("data").IsNull() || obj.Get("data").IsUndefined()) {
                ptr->data = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->data = nullptr;
            }
        }
        if (obj.Has("private_flags")) {
            ptr->private_flags = obj.Get("private_flags").As<Napi::Number>().Uint32Value();
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXString");
}

static Napi::Value Get_CXString_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXString* ptr = static_cast<CXString*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "data") {
        return wrapConstPointer(env, ptr->data, "void");
    }
    if (fieldName == "private_flags") {
        return Napi::Number::New(env, ptr->private_flags);
    }
    
    return env.Undefined();
}

static Napi::Value Create_CXStringSet(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXStringSet* ptr = new CXStringSet();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("Strings")) {
            if (obj.Get("Strings").IsObject()) {
                ptr->Strings = static_cast<CXString *>(unwrapPointer(obj.Get("Strings").As<Napi::Object>()));
            } else if (obj.Get("Strings").IsNull() || obj.Get("Strings").IsUndefined()) {
                ptr->Strings = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->Strings = nullptr;
            }
        }
        if (obj.Has("Count")) {
            ptr->Count = obj.Get("Count").As<Napi::Number>().Uint32Value();
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXStringSet");
}

static Napi::Value Get_CXStringSet_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXStringSet* ptr = static_cast<CXStringSet*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "Count") {
        return Napi::Number::New(env, ptr->Count);
    }
    if (fieldName == "Strings") {
        return wrapPointer(env, ptr->Strings, "CXString");
    }
    
    return env.Undefined();
}

// Describes a single preprocessing token.
static Napi::Value Create_CXToken(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXToken* ptr = new CXToken();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("int_data")) {
            Napi::Array int_data_arr = obj.Get("int_data").As<Napi::Array>();
            for (size_t i = 0; i < int_data_arr.Length() && i < sizeof(ptr->int_data)/sizeof(ptr->int_data[0]); i++) {
                ptr->int_data[i] = int_data_arr.Get(i).As<Napi::Number>().Uint32Value();
            }
        }
        if (obj.Has("ptr_data")) {
            if (obj.Get("ptr_data").IsExternal()) {
                ptr->ptr_data = obj.Get("ptr_data").As<Napi::External<void>>().Data();
            } else {
                ptr->ptr_data = nullptr;
            }
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXToken");
}

static Napi::Value Get_CXToken_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXToken* ptr = static_cast<CXToken*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "int_data") {
        Napi::Array arr = Napi::Array::New(env);
        size_t arraySize = sizeof(ptr->int_data)/sizeof(ptr->int_data[0]);
        for (size_t i = 0; i < arraySize; i++) {
            arr.Set(i, Napi::Number::New(env, ptr->int_data[i]));
        }
        return arr;
    }
    if (fieldName == "ptr_data") {
        return wrapPointer(env, ptr->ptr_data, "void");
    }
    
    return env.Undefined();
}

// The memory usage of a CXTranslationUnit, broken into categories.
static Napi::Value Create_CXTUResourceUsage(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXTUResourceUsage* ptr = new CXTUResourceUsage();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("data")) {
            if (obj.Get("data").IsExternal()) {
                ptr->data = obj.Get("data").As<Napi::External<void>>().Data();
            } else {
                ptr->data = nullptr;
            }
        }
        if (obj.Has("numEntries")) {
            ptr->numEntries = obj.Get("numEntries").As<Napi::Number>().Uint32Value();
        }
        if (obj.Has("entries")) {
            if (obj.Get("entries").IsObject()) {
                ptr->entries = static_cast<CXTUResourceUsageEntry *>(unwrapPointer(obj.Get("entries").As<Napi::Object>()));
            } else if (obj.Get("entries").IsNull() || obj.Get("entries").IsUndefined()) {
                ptr->entries = nullptr;
            } else {
                // Invalid value for pointer field
                ptr->entries = nullptr;
            }
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXTUResourceUsage");
}

static Napi::Value Get_CXTUResourceUsage_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXTUResourceUsage* ptr = static_cast<CXTUResourceUsage*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "data") {
        return wrapPointer(env, ptr->data, "void");
    }
    if (fieldName == "entries") {
        return wrapPointer(env, ptr->entries, "CXTUResourceUsageEntry");
    }
    if (fieldName == "numEntries") {
        return Napi::Number::New(env, ptr->numEntries);
    }
    
    return env.Undefined();
}

static Napi::Value Create_CXTUResourceUsageEntry(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXTUResourceUsageEntry* ptr = new CXTUResourceUsageEntry();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("kind")) {
            ptr->kind = static_cast<enum CXTUResourceUsageKind>(obj.Get("kind").As<Napi::Number>().Int32Value());
        }
        if (obj.Has("amount")) {
            ptr->amount = obj.Get("amount").As<Napi::Number>().Uint32Value();
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXTUResourceUsageEntry");
}

static Napi::Value Get_CXTUResourceUsageEntry_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXTUResourceUsageEntry* ptr = static_cast<CXTUResourceUsageEntry*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "amount") {
        return Napi::Number::New(env, ptr->amount);
    }
    if (fieldName == "kind") {
        return Napi::Number::New(env, static_cast<int>(ptr->kind));
    }
    
    return env.Undefined();
}

// The type of an element in the abstract syntax tree.
static Napi::Value Create_CXType(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXType* ptr = new CXType();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("kind")) {
            ptr->kind = static_cast<enum CXTypeKind>(obj.Get("kind").As<Napi::Number>().Int32Value());
        }
        if (obj.Has("data")) {
            Napi::Array data_arr = obj.Get("data").As<Napi::Array>();
            for (size_t i = 0; i < data_arr.Length() && i < sizeof(ptr->data)/sizeof(ptr->data[0]); i++) {
                if (data_arr.Get(i).IsExternal()) {
                    ptr->data[i] = data_arr.Get(i).As<Napi::External<void>>().Data();
                } else {
                    ptr->data[i] = nullptr;
                }
            }
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXType");
}

static Napi::Value Get_CXType_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXType* ptr = static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "data") {
        Napi::Array arr = Napi::Array::New(env);
        size_t arraySize = sizeof(ptr->data)/sizeof(ptr->data[0]);
        for (size_t i = 0; i < arraySize; i++) {
            arr.Set(i, Napi::External<void>::New(env, const_cast<void*>(ptr->data[i])));
        }
        return arr;
    }
    if (fieldName == "kind") {
        return Napi::Number::New(env, static_cast<int>(ptr->kind));
    }
    
    return env.Undefined();
}

// Provides the contents of a file that has not yet been saved to disk.
// 
// Each CXUnsavedFile instance provides the name of a file on the
// system along with the current contents of that file that have not
// yet been saved to disk.
static Napi::Value Create_CXUnsavedFile(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXUnsavedFile* ptr = new CXUnsavedFile();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("Filename")) {
            // Note: String lifetime management needed for production use
            std::string Filename_str = obj.Get("Filename").As<Napi::String>().Utf8Value();
            ptr->Filename = Filename_str.c_str();
        }
        if (obj.Has("Contents")) {
            // Note: String lifetime management needed for production use
            std::string Contents_str = obj.Get("Contents").As<Napi::String>().Utf8Value();
            ptr->Contents = Contents_str.c_str();
        }
        if (obj.Has("Length")) {
            ptr->Length = obj.Get("Length").As<Napi::Number>().Uint32Value();
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXUnsavedFile");
}

static Napi::Value Get_CXUnsavedFile_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXUnsavedFile* ptr = static_cast<CXUnsavedFile*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "Contents") {
        return Napi::String::New(env, ptr->Contents);
    }
    if (fieldName == "Filename") {
        return Napi::String::New(env, ptr->Filename);
    }
    if (fieldName == "Length") {
        return Napi::Number::New(env, ptr->Length);
    }
    
    return env.Undefined();
}

// Describes a version number of the form major.minor.subminor.
static Napi::Value Create_CXVersion(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    CXVersion* ptr = new CXVersion();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("Major")) {
            ptr->Major = obj.Get("Major").As<Napi::Number>().Int32Value();
        }
        if (obj.Has("Minor")) {
            ptr->Minor = obj.Get("Minor").As<Napi::Number>().Int32Value();
        }
        if (obj.Has("Subminor")) {
            ptr->Subminor = obj.Get("Subminor").As<Napi::Number>().Int32Value();
        }
    }
    
    return wrapOwnedPointer(env, ptr, "CXVersion");
}

static Napi::Value Get_CXVersion_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    CXVersion* ptr = static_cast<CXVersion*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "Major") {
        return Napi::Number::New(env, ptr->Major);
    }
    if (fieldName == "Minor") {
        return Napi::Number::New(env, ptr->Minor);
    }
    if (fieldName == "Subminor") {
        return Napi::Number::New(env, ptr->Subminor);
    }
    
    return env.Undefined();
}

// A group of callbacks used by #clang_indexSourceFile and
// #clang_indexTranslationUnit.
static Napi::Value Create_IndexerCallbacks(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    // Allocate struct
    IndexerCallbacks* ptr = new IndexerCallbacks();
    
    // Initialize from JavaScript object if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("abortQuery")) {
            if (obj.Get("abortQuery").IsExternal()) {
                ptr->abortQuery = reinterpret_cast<int (*)(CXClientData, void *)>(obj.Get("abortQuery").As<Napi::External<void>>().Data());
            } else {
                ptr->abortQuery = nullptr;
            }
        }
        if (obj.Has("diagnostic")) {
            if (obj.Get("diagnostic").IsExternal()) {
                ptr->diagnostic = reinterpret_cast<void (*)(CXClientData, CXDiagnosticSet, void *)>(obj.Get("diagnostic").As<Napi::External<void>>().Data());
            } else {
                ptr->diagnostic = nullptr;
            }
        }
        if (obj.Has("enteredMainFile")) {
            if (obj.Get("enteredMainFile").IsExternal()) {
                ptr->enteredMainFile = reinterpret_cast<CXIdxClientFile (*)(CXClientData, CXFile, void *)>(obj.Get("enteredMainFile").As<Napi::External<void>>().Data());
            } else {
                ptr->enteredMainFile = nullptr;
            }
        }
        if (obj.Has("ppIncludedFile")) {
            if (obj.Get("ppIncludedFile").IsExternal()) {
                ptr->ppIncludedFile = reinterpret_cast<CXIdxClientFile (*)(CXClientData, const CXIdxIncludedFileInfo *)>(obj.Get("ppIncludedFile").As<Napi::External<void>>().Data());
            } else {
                ptr->ppIncludedFile = nullptr;
            }
        }
        if (obj.Has("importedASTFile")) {
            if (obj.Get("importedASTFile").IsExternal()) {
                ptr->importedASTFile = reinterpret_cast<CXIdxClientASTFile (*)(CXClientData, const CXIdxImportedASTFileInfo *)>(obj.Get("importedASTFile").As<Napi::External<void>>().Data());
            } else {
                ptr->importedASTFile = nullptr;
            }
        }
        if (obj.Has("startedTranslationUnit")) {
            if (obj.Get("startedTranslationUnit").IsExternal()) {
                ptr->startedTranslationUnit = reinterpret_cast<CXIdxClientContainer (*)(CXClientData, void *)>(obj.Get("startedTranslationUnit").As<Napi::External<void>>().Data());
            } else {
                ptr->startedTranslationUnit = nullptr;
            }
        }
        if (obj.Has("indexDeclaration")) {
            if (obj.Get("indexDeclaration").IsExternal()) {
                ptr->indexDeclaration = reinterpret_cast<void (*)(CXClientData, const CXIdxDeclInfo *)>(obj.Get("indexDeclaration").As<Napi::External<void>>().Data());
            } else {
                ptr->indexDeclaration = nullptr;
            }
        }
        if (obj.Has("indexEntityReference")) {
            if (obj.Get("indexEntityReference").IsExternal()) {
                ptr->indexEntityReference = reinterpret_cast<void (*)(CXClientData, const CXIdxEntityRefInfo *)>(obj.Get("indexEntityReference").As<Napi::External<void>>().Data());
            } else {
                ptr->indexEntityReference = nullptr;
            }
        }
    }
    
    return wrapOwnedPointer(env, ptr, "IndexerCallbacks");
}

static Napi::Value Get_IndexerCallbacks_Field(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    IndexerCallbacks* ptr = static_cast<IndexerCallbacks*>(unwrapPointer(info[0].As<Napi::Object>()));
    std::string fieldName = info[1].As<Napi::String>().Utf8Value();
    
    if (fieldName == "abortQuery") {
        return Napi::External<void>::New(env, reinterpret_cast<void*>(ptr->abortQuery));
    }
    if (fieldName == "diagnostic") {
        return Napi::External<void>::New(env, reinterpret_cast<void*>(ptr->diagnostic));
    }
    if (fieldName == "enteredMainFile") {
        return Napi::External<void>::New(env, reinterpret_cast<void*>(ptr->enteredMainFile));
    }
    if (fieldName == "importedASTFile") {
        return Napi::External<void>::New(env, reinterpret_cast<void*>(ptr->importedASTFile));
    }
    if (fieldName == "indexDeclaration") {
        return Napi::External<void>::New(env, reinterpret_cast<void*>(ptr->indexDeclaration));
    }
    if (fieldName == "indexEntityReference") {
        return Napi::External<void>::New(env, reinterpret_cast<void*>(ptr->indexEntityReference));
    }
    if (fieldName == "ppIncludedFile") {
        return Napi::External<void>::New(env, reinterpret_cast<void*>(ptr->ppIncludedFile));
    }
    if (fieldName == "startedTranslationUnit") {
        return Napi::External<void>::New(env, reinterpret_cast<void*>(ptr->startedTranslationUnit));
    }
    
    return env.Undefined();
}


// Function wrappers
// Annotate the given set of tokens by providing cursors for each token
// that can be mapped to a specific entity within the abstract syntax tree.
// 
// This token-annotation routine is equivalent to invoking
// clang_getCursor() for the source locations of each of the
// tokens. The cursors provided are filtered, so that only those
// cursors that have a direct correspondence to the token are
// accepted. For example, given a function call \c f(x),
// clang_getCursor() would provide the following cursors:
// 
// * when the cursor is over the 'f', a DeclRefExpr cursor referring to 'f'.
// * when the cursor is over the '(' or the ')', a CallExpr referring to 'f'.
// * when the cursor is over the 'x', a DeclRefExpr cursor referring to 'x'.
// 
// Only the first and last of these cursors will occur within the
// annotate, since the tokens "f" and "x' directly refer to a function
// and a variable, respectively, but the parentheses are just a small
// part of the full syntax of the function call expression, which is
// not provided as an annotation.
// 
// \param TU the translation unit that owns the given tokens.
// 
// \param Tokens the set of tokens to annotate.
// 
// \param NumTokens the number of tokens in \p Tokens.
// 
// \param Cursors an array of \p NumTokens cursors, whose contents will be
// replaced with the cursors corresponding to each token.
static Napi::Value clang_annotateTokens_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 4) {
        Napi::TypeError::New(env, "Expected 4 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: TU (CXTranslationUnit)
    CXTranslationUnit TU = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: Tokens (CXToken *)
    CXToken * Tokens = nullptr;
    if (!info[1].IsNull() && !info[1].IsUndefined() && info[1].IsObject()) {
        Tokens = static_cast<CXToken *>(unwrapPointer(info[1].As<Napi::Object>()));
    }
    // Parameter: NumTokens (unsigned int)
    auto NumTokens = info[2].As<Napi::Number>().Uint32Value();
    // Parameter: Cursors (CXCursor *)
    CXCursor * Cursors = nullptr;
    if (!info[3].IsNull() && !info[3].IsUndefined() && info[3].IsObject()) {
        Cursors = static_cast<CXCursor *>(unwrapPointer(info[3].As<Napi::Object>()));
    }
    
    clang_annotateTokens(TU, Tokens, NumTokens, Cursors);
    return env.Undefined();
}

// Perform code completion at a given location in a translation unit.
// 
// This function performs code completion at a particular file, line, and
// column within source code, providing results that suggest potential
// code snippets based on the context of the completion. The basic model
// for code completion is that Clang will parse a complete source file,
// performing syntax checking up to the location where code-completion has
// been requested. At that point, a special code-completion token is passed
// to the parser, which recognizes this token and determines, based on the
// current location in the C/Objective-C/C++ grammar and the state of
// semantic analysis, what completions to provide. These completions are
// returned via a new \c CXCodeCompleteResults structure.
// 
// Code completion itself is meant to be triggered by the client when the
// user types punctuation characters or whitespace, at which point the
// code-completion location will coincide with the cursor. For example, if \c p
// is a pointer, code-completion might be triggered after the "-" and then
// after the ">" in \c p->. When the code-completion location is after the ">",
// the completion results will provide, e.g., the members of the struct that
// "p" points to. The client is responsible for placing the cursor at the
// beginning of the token currently being typed, then filtering the results
// based on the contents of the token. For example, when code-completing for
// the expression \c p->get, the client should provide the location just after
// the ">" (e.g., pointing at the "g") to this code-completion hook. Then, the
// client can filter the results based on the current token text ("get"), only
// showing those results that start with "get". The intent of this interface
// is to separate the relatively high-latency acquisition of code-completion
// results from the filtering of results on a per-character basis, which must
// have a lower latency.
// 
// \param TU The translation unit in which code-completion should
// occur. The source files for this translation unit need not be
// completely up-to-date (and the contents of those source files may
// be overridden via \p unsaved_files). Cursors referring into the
// translation unit may be invalidated by this invocation.
// 
// \param complete_filename The name of the source file where code
// completion should be performed. This filename may be any file
// included in the translation unit.
// 
// \param complete_line The line at which code-completion should occur.
// 
// \param complete_column The column at which code-completion should occur.
// Note that the column should point just after the syntactic construct that
// initiated code completion, and not in the middle of a lexical token.
// 
// \param unsaved_files the Files that have not yet been saved to disk
// but may be required for parsing or code completion, including the
// contents of those files.  The contents and name of these files (as
// specified by CXUnsavedFile) are copied when necessary, so the
// client only needs to guarantee their validity until the call to
// this function returns.
// 
// \param num_unsaved_files The number of unsaved file entries in \p
// unsaved_files.
// 
// \param options Extra options that control the behavior of code
// completion, expressed as a bitwise OR of the enumerators of the
// CXCodeComplete_Flags enumeration. The
// \c clang_defaultCodeCompleteOptions() function returns a default set
// of code-completion options.
// 
// \returns If successful, a new \c CXCodeCompleteResults structure
// containing code-completion results, which should eventually be
// freed with \c clang_disposeCodeCompleteResults(). If code
// completion fails, returns NULL.
static Napi::Value clang_codeCompleteAt_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 7) {
        Napi::TypeError::New(env, "Expected 7 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: TU (CXTranslationUnit)
    CXTranslationUnit TU = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: complete_filename (const char *)
    std::string complete_filename_str = info[1].As<Napi::String>().Utf8Value();
    const char* complete_filename = complete_filename_str.c_str();
    // Parameter: complete_line (unsigned int)
    auto complete_line = info[2].As<Napi::Number>().Uint32Value();
    // Parameter: complete_column (unsigned int)
    auto complete_column = info[3].As<Napi::Number>().Uint32Value();
    // Parameter: unsaved_files (struct CXUnsavedFile *)
    struct CXUnsavedFile * unsaved_files = nullptr;
    if (!info[4].IsNull() && !info[4].IsUndefined() && info[4].IsObject()) {
        unsaved_files = static_cast<struct CXUnsavedFile *>(unwrapPointer(info[4].As<Napi::Object>()));
    }
    // Parameter: num_unsaved_files (unsigned int)
    auto num_unsaved_files = info[5].As<Napi::Number>().Uint32Value();
    // Parameter: options (unsigned int)
    auto options = info[6].As<Napi::Number>().Uint32Value();
    
    CXCodeCompleteResults * result = clang_codeCompleteAt(TU, complete_filename, complete_line, complete_column, unsaved_files, num_unsaved_files, options);
    CXCodeCompleteResults ** resultPtr = new CXCodeCompleteResults *;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXCodeCompleteResults *");
}

// Returns the cursor kind for the container for the current code
// completion context. The container is only guaranteed to be set for
// contexts where a container exists (i.e. member accesses or Objective-C
// message sends); if there is not a container, this function will return
// CXCursor_InvalidCode.
// 
// \param Results the code completion results to query
// 
// \param IsIncomplete on return, this value will be false if Clang has complete
// information about the container. If Clang does not have complete
// information, this value will be true.
// 
// \returns the container kind, or CXCursor_InvalidCode if there is not a
// container
static Napi::Value clang_codeCompleteGetContainerKind_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Results (CXCodeCompleteResults *)
    CXCodeCompleteResults * Results = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        Results = static_cast<CXCodeCompleteResults *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    // Parameter: IsIncomplete (unsigned int *)
    unsigned int * IsIncomplete = *static_cast<unsigned int **>(unwrapPointer(info[1].As<Napi::Object>()));
    
    auto result = clang_codeCompleteGetContainerKind(Results, IsIncomplete);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Returns the USR for the container for the current code completion
// context. If there is not a container for the current context, this
// function will return the empty string.
// 
// \param Results the code completion results to query
// 
// \returns the USR for the container
static Napi::Value clang_codeCompleteGetContainerUSR_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Results (CXCodeCompleteResults *)
    CXCodeCompleteResults * Results = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        Results = static_cast<CXCodeCompleteResults *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    
    CXString result = clang_codeCompleteGetContainerUSR(Results);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Determines what completions are appropriate for the context
// the given code completion.
// 
// \param Results the code completion results to query
// 
// \returns the kinds of completions that are appropriate for use
// along with the given code completion results.
static Napi::Value clang_codeCompleteGetContexts_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Results (CXCodeCompleteResults *)
    CXCodeCompleteResults * Results = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        Results = static_cast<CXCodeCompleteResults *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    
    auto result = clang_codeCompleteGetContexts(Results);
    return Napi::BigInt::New(env, result);
}

// Retrieve a diagnostic associated with the given code completion.
// 
// \param Results the code completion results to query.
// \param Index the zero-based diagnostic number to retrieve.
// 
// \returns the requested diagnostic. This diagnostic must be freed
// via a call to \c clang_disposeDiagnostic().
static Napi::Value clang_codeCompleteGetDiagnostic_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Results (CXCodeCompleteResults *)
    CXCodeCompleteResults * Results = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        Results = static_cast<CXCodeCompleteResults *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    // Parameter: Index (unsigned int)
    auto Index = info[1].As<Napi::Number>().Uint32Value();
    
    CXDiagnostic result = clang_codeCompleteGetDiagnostic(Results, Index);
    CXDiagnostic* resultPtr = new CXDiagnostic;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXDiagnostic");
}

// Determine the number of diagnostics produced prior to the
// location where code completion was performed.
static Napi::Value clang_codeCompleteGetNumDiagnostics_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Results (CXCodeCompleteResults *)
    CXCodeCompleteResults * Results = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        Results = static_cast<CXCodeCompleteResults *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    
    auto result = clang_codeCompleteGetNumDiagnostics(Results);
    return Napi::Number::New(env, result);
}

// Returns the currently-entered selector for an Objective-C message
// send, formatted like "initWithFoo:bar:". Only guaranteed to return a
// non-empty string for CXCompletionContext_ObjCInstanceMessage and
// CXCompletionContext_ObjCClassMessage.
// 
// \param Results the code completion results to query
// 
// \returns the selector (or partial selector) that has been entered thus far
// for an Objective-C message send.
static Napi::Value clang_codeCompleteGetObjCSelector_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Results (CXCodeCompleteResults *)
    CXCodeCompleteResults * Results = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        Results = static_cast<CXCodeCompleteResults *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    
    CXString result = clang_codeCompleteGetObjCSelector(Results);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Construct a USR for a specified Objective-C category.
static Napi::Value clang_constructUSR_ObjCCategory_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: class_name (const char *)
    std::string class_name_str = info[0].As<Napi::String>().Utf8Value();
    const char* class_name = class_name_str.c_str();
    // Parameter: category_name (const char *)
    std::string category_name_str = info[1].As<Napi::String>().Utf8Value();
    const char* category_name = category_name_str.c_str();
    
    CXString result = clang_constructUSR_ObjCCategory(class_name, category_name);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Construct a USR for a specified Objective-C class.
static Napi::Value clang_constructUSR_ObjCClass_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: class_name (const char *)
    std::string class_name_str = info[0].As<Napi::String>().Utf8Value();
    const char* class_name = class_name_str.c_str();
    
    CXString result = clang_constructUSR_ObjCClass(class_name);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Construct a USR for a specified Objective-C instance variable and
// the USR for its containing class.
static Napi::Value clang_constructUSR_ObjCIvar_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: name (const char *)
    std::string name_str = info[0].As<Napi::String>().Utf8Value();
    const char* name = name_str.c_str();
    // Parameter: classUSR (CXString)
    CXString classUSR = *static_cast<CXString*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    CXString result = clang_constructUSR_ObjCIvar(name, classUSR);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Construct a USR for a specified Objective-C method and
// the USR for its containing class.
static Napi::Value clang_constructUSR_ObjCMethod_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: name (const char *)
    std::string name_str = info[0].As<Napi::String>().Utf8Value();
    const char* name = name_str.c_str();
    // Parameter: isInstanceMethod (unsigned int)
    auto isInstanceMethod = info[1].As<Napi::Number>().Uint32Value();
    // Parameter: classUSR (CXString)
    CXString classUSR = *static_cast<CXString*>(unwrapPointer(info[2].As<Napi::Object>()));
    
    CXString result = clang_constructUSR_ObjCMethod(name, isInstanceMethod, classUSR);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Construct a USR for a specified Objective-C property and the USR
// for its containing class.
static Napi::Value clang_constructUSR_ObjCProperty_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: property (const char *)
    std::string property_str = info[0].As<Napi::String>().Utf8Value();
    const char* property = property_str.c_str();
    // Parameter: classUSR (CXString)
    CXString classUSR = *static_cast<CXString*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    CXString result = clang_constructUSR_ObjCProperty(property, classUSR);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Construct a USR for a specified Objective-C protocol.
static Napi::Value clang_constructUSR_ObjCProtocol_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: protocol_name (const char *)
    std::string protocol_name_str = info[0].As<Napi::String>().Utf8Value();
    const char* protocol_name = protocol_name_str.c_str();
    
    CXString result = clang_constructUSR_ObjCProtocol(protocol_name);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Creates an empty CXCursorSet.
static Napi::Value clang_createCXCursorSet_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    
    CXCursorSet result = clang_createCXCursorSet();
    CXCursorSet* resultPtr = new CXCursorSet;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXCursorSet");
}

// Provides a shared context for creating translation units.
// 
// It provides two options:
// 
// - excludeDeclarationsFromPCH: When non-zero, allows enumeration of "local"
// declarations (when loading any new translation units). A "local" declaration
// is one that belongs in the translation unit itself and not in a precompiled
// header that was used by the translation unit. If zero, all declarations
// will be enumerated.
// 
// Here is an example:
// 
// \code
// // excludeDeclsFromPCH = 1, displayDiagnostics=1
// Idx = clang_createIndex(1, 1);
// 
// // IndexTest.pch was produced with the following command:
// // "clang -x c IndexTest.h -emit-ast -o IndexTest.pch"
// TU = clang_createTranslationUnit(Idx, "IndexTest.pch");
// 
// // This will load all the symbols from 'IndexTest.pch'
// clang_visitChildren(clang_getTranslationUnitCursor(TU),
// TranslationUnitVisitor, 0);
// clang_disposeTranslationUnit(TU);
// 
// // This will load all the symbols from 'IndexTest.c', excluding symbols
// // from 'IndexTest.pch'.
// char *args[] = { "-Xclang", "-include-pch=IndexTest.pch" };
// TU = clang_createTranslationUnitFromSourceFile(Idx, "IndexTest.c", 2, args,
// 0, 0);
// clang_visitChildren(clang_getTranslationUnitCursor(TU),
// TranslationUnitVisitor, 0);
// clang_disposeTranslationUnit(TU);
// \endcode
// 
// This process of creating the 'pch', loading it separately, and using it (via
// -include-pch) allows 'excludeDeclsFromPCH' to remove redundant callbacks
// (which gives the indexer the same performance benefit as the compiler).
static Napi::Value clang_createIndex_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: excludeDeclarationsFromPCH (int)
    auto excludeDeclarationsFromPCH = info[0].As<Napi::Number>().Int32Value();
    // Parameter: displayDiagnostics (int)
    auto displayDiagnostics = info[1].As<Napi::Number>().Int32Value();
    
    CXIndex result = clang_createIndex(excludeDeclarationsFromPCH, displayDiagnostics);
    CXIndex* resultPtr = new CXIndex;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXIndex");
}

// Provides a shared context for creating translation units.
// 
// Call this function instead of clang_createIndex() if you need to configure
// the additional options in CXIndexOptions.
// 
// \returns The created index or null in case of error, such as an unsupported
// value of options->Size.
// 
// For example:
// \code
// CXIndex createIndex(const char *ApplicationTemporaryPath) {
// const int ExcludeDeclarationsFromPCH = 1;
// const int DisplayDiagnostics = 1;
// CXIndex Idx;
// #if CINDEX_VERSION_MINOR >= 64
// CXIndexOptions Opts;
// memset(&Opts, 0, sizeof(Opts));
// Opts.Size = sizeof(CXIndexOptions);
// Opts.ThreadBackgroundPriorityForIndexing = 1;
// Opts.ExcludeDeclarationsFromPCH = ExcludeDeclarationsFromPCH;
// Opts.DisplayDiagnostics = DisplayDiagnostics;
// Opts.PreambleStoragePath = ApplicationTemporaryPath;
// Idx = clang_createIndexWithOptions(&Opts);
// if (Idx)
// return Idx;
// fprintf(stderr,
// "clang_createIndexWithOptions() failed. "
// "CINDEX_VERSION_MINOR = %d, sizeof(CXIndexOptions) = %u\n",
// CINDEX_VERSION_MINOR, Opts.Size);
// #else
// (void)ApplicationTemporaryPath;
// #endif
// Idx = clang_createIndex(ExcludeDeclarationsFromPCH, DisplayDiagnostics);
// clang_CXIndex_setGlobalOptions(
// Idx, clang_CXIndex_getGlobalOptions(Idx) |
// CXGlobalOpt_ThreadBackgroundPriorityForIndexing);
// return Idx;
// }
// \endcode
// 
// \sa clang_createIndex()
static Napi::Value clang_createIndexWithOptions_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: options (const CXIndexOptions *)
    const CXIndexOptions * options = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        options = static_cast<const CXIndexOptions *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    
    CXIndex result = clang_createIndexWithOptions(options);
    CXIndex* resultPtr = new CXIndex;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXIndex");
}

// Same as \c clang_createTranslationUnit2, but returns
// the \c CXTranslationUnit instead of an error code.  In case of an error this
// routine returns a \c NULL \c CXTranslationUnit, without further detailed
// error codes.
static Napi::Value clang_createTranslationUnit_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: CIdx (CXIndex)
    CXIndex CIdx = *static_cast<CXIndex*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: ast_filename (const char *)
    std::string ast_filename_str = info[1].As<Napi::String>().Utf8Value();
    const char* ast_filename = ast_filename_str.c_str();
    
    CXTranslationUnit result = clang_createTranslationUnit(CIdx, ast_filename);
    CXTranslationUnit* resultPtr = new CXTranslationUnit;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXTranslationUnit");
}

// Create a translation unit from an AST file (\c -emit-ast).
// 
// \param[out] out_TU A non-NULL pointer to store the created
// \c CXTranslationUnit.
// 
// \returns Zero on success, otherwise returns an error code.
static Napi::Value clang_createTranslationUnit2_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: CIdx (CXIndex)
    CXIndex CIdx = *static_cast<CXIndex*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: ast_filename (const char *)
    std::string ast_filename_str = info[1].As<Napi::String>().Utf8Value();
    const char* ast_filename = ast_filename_str.c_str();
    // Parameter: out_TU (CXTranslationUnit *)
    CXTranslationUnit * out_TU = *static_cast<CXTranslationUnit **>(unwrapPointer(info[2].As<Napi::Object>()));
    
    auto result = clang_createTranslationUnit2(CIdx, ast_filename, out_TU);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Return the CXTranslationUnit for a given source file and the provided
// command line arguments one would pass to the compiler.
// 
// Note: The 'source_filename' argument is optional.  If the caller provides a
// NULL pointer, the name of the source file is expected to reside in the
// specified command line arguments.
// 
// Note: When encountered in 'clang_command_line_args', the following options
// are ignored:
// 
// '-c'
// '-emit-ast'
// '-fsyntax-only'
// '-o \<output file>'  (both '-o' and '\<output file>' are ignored)
// 
// \param CIdx The index object with which the translation unit will be
// associated.
// 
// \param source_filename The name of the source file to load, or NULL if the
// source file is included in \p clang_command_line_args.
// 
// \param num_clang_command_line_args The number of command-line arguments in
// \p clang_command_line_args.
// 
// \param clang_command_line_args The command-line arguments that would be
// passed to the \c clang executable if it were being invoked out-of-process.
// These command-line options will be parsed and will affect how the translation
// unit is parsed. Note that the following options are ignored: '-c',
// '-emit-ast', '-fsyntax-only' (which is the default), and '-o \<output file>'.
// 
// \param num_unsaved_files the number of unsaved file entries in \p
// unsaved_files.
// 
// \param unsaved_files the files that have not yet been saved to disk
// but may be required for code completion, including the contents of
// those files.  The contents and name of these files (as specified by
// CXUnsavedFile) are copied when necessary, so the client only needs to
// guarantee their validity until the call to this function returns.
static Napi::Value clang_createTranslationUnitFromSourceFile_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 6) {
        Napi::TypeError::New(env, "Expected 6 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: CIdx (CXIndex)
    CXIndex CIdx = *static_cast<CXIndex*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: source_filename (const char *)
    std::string source_filename_str = info[1].As<Napi::String>().Utf8Value();
    const char* source_filename = source_filename_str.c_str();
    // Parameter: num_clang_command_line_args (int)
    auto num_clang_command_line_args = info[2].As<Napi::Number>().Int32Value();
    // Parameter: clang_command_line_args (const char *const *)
    const char *const * clang_command_line_args = nullptr;
    std::vector<std::string> clang_command_line_args_strings;
    std::vector<const char*> clang_command_line_args_cstrs;
    if (!info[3].IsNull() && !info[3].IsUndefined()) {
        if (info[3].IsArray()) {
            Napi::Array arr = info[3].As<Napi::Array>();
            for (uint32_t j = 0; j < arr.Length(); j++) {
                clang_command_line_args_strings.push_back(arr.Get(j).As<Napi::String>().Utf8Value());
            }
            for (const auto& s : clang_command_line_args_strings) {
                clang_command_line_args_cstrs.push_back(s.c_str());
            }
            clang_command_line_args = clang_command_line_args_cstrs.data();
        } else if (info[3].IsObject()) {
            // Assume it's a wrapped pointer
            clang_command_line_args = *static_cast<const char *const **>(unwrapPointer(info[3].As<Napi::Object>()));
        }
    }
    // Parameter: num_unsaved_files (unsigned int)
    auto num_unsaved_files = info[4].As<Napi::Number>().Uint32Value();
    // Parameter: unsaved_files (struct CXUnsavedFile *)
    struct CXUnsavedFile * unsaved_files = nullptr;
    if (!info[5].IsNull() && !info[5].IsUndefined() && info[5].IsObject()) {
        unsaved_files = static_cast<struct CXUnsavedFile *>(unwrapPointer(info[5].As<Napi::Object>()));
    }
    
    CXTranslationUnit result = clang_createTranslationUnitFromSourceFile(CIdx, source_filename, num_clang_command_line_args, clang_command_line_args, num_unsaved_files, unsaved_files);
    CXTranslationUnit* resultPtr = new CXTranslationUnit;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXTranslationUnit");
}

// If cursor is a statement declaration tries to evaluate the
// statement and if its variable, tries to evaluate its initializer,
// into its corresponding type.
// If it's an expression, tries to evaluate the expression.
static Napi::Value clang_Cursor_Evaluate_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXEvalResult result = clang_Cursor_Evaluate(C);
    CXEvalResult* resultPtr = new CXEvalResult;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXEvalResult");
}

// Retrieve the argument cursor of a function or method.
// 
// The argument cursor can be determined for calls as well as for declarations
// of functions or methods. For other cursors and for invalid indices, an
// invalid cursor is returned.
static Napi::Value clang_Cursor_getArgument_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: i (unsigned int)
    auto i = info[1].As<Napi::Number>().Uint32Value();
    
    CXCursor result = clang_Cursor_getArgument(C, i);
    CXCursor* resultPtr = new CXCursor(result);
    return wrapOwnedPointer(env, resultPtr, "CXCursor");
}

// \brief Returns the operator code for the binary operator.
static Napi::Value clang_Cursor_getBinaryOpcode_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_getBinaryOpcode(C);
    return Napi::Number::New(env, static_cast<int>(result));
}

// \brief Returns a string containing the spelling of the binary operator.
static Napi::Value clang_Cursor_getBinaryOpcodeStr_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Op (enum CX_BinaryOperatorKind)
    enum CX_BinaryOperatorKind Op = static_cast<enum CX_BinaryOperatorKind>(info[0].As<Napi::Number>().Int32Value());
    
    CXString result = clang_Cursor_getBinaryOpcodeStr(Op);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Given a cursor that represents a documentable entity (e.g.,
// declaration), return the associated \paragraph; otherwise return the
// first paragraph.
static Napi::Value clang_Cursor_getBriefCommentText_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_Cursor_getBriefCommentText(C);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Given a cursor that represents a declaration, return the associated
// comment's source range.  The range may include multiple consecutive comments
// with whitespace in between.
static Napi::Value clang_Cursor_getCommentRange_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXSourceRange result = clang_Cursor_getCommentRange(C);
    CXSourceRange* resultPtr = new CXSourceRange(result);
    return wrapOwnedPointer(env, resultPtr, "CXSourceRange");
}

// Retrieve the CXStrings representing the mangled symbols of the C++
// constructor or destructor at the cursor.
static Napi::Value clang_Cursor_getCXXManglings_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXStringSet * result = clang_Cursor_getCXXManglings(arg0);
    CXStringSet ** resultPtr = new CXStringSet *;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXStringSet *");
}

// Retrieve the CXString representing the mangled name of the cursor.
static Napi::Value clang_Cursor_getMangling_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_Cursor_getMangling(arg0);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Given a CXCursor_ModuleImportDecl cursor, return the associated module.
static Napi::Value clang_Cursor_getModule_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXModule result = clang_Cursor_getModule(C);
    CXModule* resultPtr = new CXModule;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXModule");
}

// Retrieve the number of non-variadic arguments associated with a given
// cursor.
// 
// The number of arguments can be determined for calls as well as for
// declarations of functions or methods. For other cursors -1 is returned.
static Napi::Value clang_Cursor_getNumArguments_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_getNumArguments(C);
    return Napi::Number::New(env, result);
}

// Returns the number of template args of a function, struct, or class decl
// representing a template specialization.
// 
// If the argument cursor cannot be converted into a template function
// declaration, -1 is returned.
// 
// For example, for the following declaration and specialization:
// template <typename T, int kInt, bool kBool>
// void foo() { ... }
// 
// template <>
// void foo<float, -7, true>();
// 
// The value 3 would be returned from this call.
static Napi::Value clang_Cursor_getNumTemplateArguments_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_getNumTemplateArguments(C);
    return Napi::Number::New(env, result);
}

// Given a cursor that represents an Objective-C method or parameter
// declaration, return the associated Objective-C qualifiers for the return
// type or the parameter respectively. The bits are formed from
// CXObjCDeclQualifierKind.
static Napi::Value clang_Cursor_getObjCDeclQualifiers_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_getObjCDeclQualifiers(C);
    return Napi::Number::New(env, result);
}

// Retrieve the CXStrings representing the mangled symbols of the ObjC
// class interface or implementation at the cursor.
static Napi::Value clang_Cursor_getObjCManglings_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXStringSet * result = clang_Cursor_getObjCManglings(arg0);
    CXStringSet ** resultPtr = new CXStringSet *;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXStringSet *");
}

// Given a cursor that represents a property declaration, return the
// associated property attributes. The bits are formed from
// \c CXObjCPropertyAttrKind.
// 
// \param reserved Reserved for future use, pass 0.
static Napi::Value clang_Cursor_getObjCPropertyAttributes_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: reserved (unsigned int)
    auto reserved = info[1].As<Napi::Number>().Uint32Value();
    
    auto result = clang_Cursor_getObjCPropertyAttributes(C, reserved);
    return Napi::Number::New(env, result);
}

// Given a cursor that represents a property declaration, return the
// name of the method that implements the getter.
static Napi::Value clang_Cursor_getObjCPropertyGetterName_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_Cursor_getObjCPropertyGetterName(C);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Given a cursor that represents a property declaration, return the
// name of the method that implements the setter, if any.
static Napi::Value clang_Cursor_getObjCPropertySetterName_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_Cursor_getObjCPropertySetterName(C);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// If the cursor points to a selector identifier in an Objective-C
// method or message expression, this returns the selector index.
// 
// After getting a cursor with #clang_getCursor, this can be called to
// determine if the location points to a selector identifier.
// 
// \returns The selector index if the cursor is an Objective-C method or message
// expression and the cursor is pointing to a selector identifier, or -1
// otherwise.
static Napi::Value clang_Cursor_getObjCSelectorIndex_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_getObjCSelectorIndex(arg0);
    return Napi::Number::New(env, result);
}

// Return the offset of the field represented by the Cursor.
// 
// If the cursor is not a field declaration, -1 is returned.
// If the cursor semantic parent is not a record field declaration,
// CXTypeLayoutError_Invalid is returned.
// If the field's type declaration is an incomplete type,
// CXTypeLayoutError_Incomplete is returned.
// If the field's type declaration is a dependent type,
// CXTypeLayoutError_Dependent is returned.
// If the field's name S is not found,
// CXTypeLayoutError_InvalidFieldName is returned.
static Napi::Value clang_Cursor_getOffsetOfField_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_getOffsetOfField(C);
    return Napi::BigInt::New(env, result);
}

// Given a cursor that represents a declaration, return the associated
// comment text, including comment markers.
static Napi::Value clang_Cursor_getRawCommentText_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_Cursor_getRawCommentText(C);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Given a cursor pointing to an Objective-C message or property
// reference, or C++ method call, returns the CXType of the receiver.
static Napi::Value clang_Cursor_getReceiverType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXType result = clang_Cursor_getReceiverType(C);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Retrieve a range for a piece that forms the cursors spelling name.
// Most of the times there is only one range for the complete spelling but for
// Objective-C methods and Objective-C message expressions, there are multiple
// pieces for each selector identifier.
// 
// \param pieceIndex the index of the spelling name piece. If this is greater
// than the actual number of pieces, it will return a NULL (invalid) range.
// 
// \param options Reserved.
static Napi::Value clang_Cursor_getSpellingNameRange_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: pieceIndex (unsigned int)
    auto pieceIndex = info[1].As<Napi::Number>().Uint32Value();
    // Parameter: options (unsigned int)
    auto options = info[2].As<Napi::Number>().Uint32Value();
    
    CXSourceRange result = clang_Cursor_getSpellingNameRange(arg0, pieceIndex, options);
    CXSourceRange* resultPtr = new CXSourceRange(result);
    return wrapOwnedPointer(env, resultPtr, "CXSourceRange");
}

// Returns the storage class for a function or variable declaration.
// 
// If the passed in Cursor is not a function or variable declaration,
// CX_SC_Invalid is returned else the storage class.
static Napi::Value clang_Cursor_getStorageClass_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_getStorageClass(arg0);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Retrieve the kind of the I'th template argument of the CXCursor C.
// 
// If the argument CXCursor does not represent a FunctionDecl, StructDecl, or
// ClassTemplatePartialSpecialization, an invalid template argument kind is
// returned.
// 
// For example, for the following declaration and specialization:
// template <typename T, int kInt, bool kBool>
// void foo() { ... }
// 
// template <>
// void foo<float, -7, true>();
// 
// For I = 0, 1, and 2, Type, Integral, and Integral will be returned,
// respectively.
static Napi::Value clang_Cursor_getTemplateArgumentKind_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: I (unsigned int)
    auto I = info[1].As<Napi::Number>().Uint32Value();
    
    auto result = clang_Cursor_getTemplateArgumentKind(C, I);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Retrieve a CXType representing the type of a TemplateArgument of a
// function decl representing a template specialization.
// 
// If the argument CXCursor does not represent a FunctionDecl, StructDecl,
// ClassDecl or ClassTemplatePartialSpecialization whose I'th template argument
// has a kind of CXTemplateArgKind_Integral, an invalid type is returned.
// 
// For example, for the following declaration and specialization:
// template <typename T, int kInt, bool kBool>
// void foo() { ... }
// 
// template <>
// void foo<float, -7, true>();
// 
// If called with I = 0, "float", will be returned.
// Invalid types will be returned for I == 1 or 2.
static Napi::Value clang_Cursor_getTemplateArgumentType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: I (unsigned int)
    auto I = info[1].As<Napi::Number>().Uint32Value();
    
    CXType result = clang_Cursor_getTemplateArgumentType(C, I);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Retrieve the value of an Integral TemplateArgument (of a function
// decl representing a template specialization) as an unsigned long long.
// 
// It is undefined to call this function on a CXCursor that does not represent a
// FunctionDecl, StructDecl, ClassDecl or ClassTemplatePartialSpecialization or
// whose I'th template argument is not an integral value.
// 
// For example, for the following declaration and specialization:
// template <typename T, int kInt, bool kBool>
// void foo() { ... }
// 
// template <>
// void foo<float, 2147483649, true>();
// 
// If called with I = 1 or 2, 2147483649 or true will be returned, respectively.
// For I == 0, this function's behavior is undefined.
static Napi::Value clang_Cursor_getTemplateArgumentUnsignedValue_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: I (unsigned int)
    auto I = info[1].As<Napi::Number>().Uint32Value();
    
    auto result = clang_Cursor_getTemplateArgumentUnsignedValue(C, I);
    return Napi::BigInt::New(env, result);
}

// Retrieve the value of an Integral TemplateArgument (of a function
// decl representing a template specialization) as a signed long long.
// 
// It is undefined to call this function on a CXCursor that does not represent a
// FunctionDecl, StructDecl, ClassDecl or ClassTemplatePartialSpecialization
// whose I'th template argument is not an integral value.
// 
// For example, for the following declaration and specialization:
// template <typename T, int kInt, bool kBool>
// void foo() { ... }
// 
// template <>
// void foo<float, -7, true>();
// 
// If called with I = 1 or 2, -7 or true will be returned, respectively.
// For I == 0, this function's behavior is undefined.
static Napi::Value clang_Cursor_getTemplateArgumentValue_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: I (unsigned int)
    auto I = info[1].As<Napi::Number>().Uint32Value();
    
    auto result = clang_Cursor_getTemplateArgumentValue(C, I);
    return Napi::BigInt::New(env, result);
}

// Returns the translation unit that a cursor originated from.
static Napi::Value clang_Cursor_getTranslationUnit_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXTranslationUnit result = clang_Cursor_getTranslationUnit(arg0);
    CXTranslationUnit* resultPtr = new CXTranslationUnit;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXTranslationUnit");
}

// If cursor refers to a variable declaration and it has initializer returns
// cursor referring to the initializer otherwise return null cursor.
static Napi::Value clang_Cursor_getVarDeclInitializer_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXCursor result = clang_Cursor_getVarDeclInitializer(cursor);
    CXCursor* resultPtr = new CXCursor(result);
    return wrapOwnedPointer(env, resultPtr, "CXCursor");
}

// Determine whether the given cursor has any attributes.
static Napi::Value clang_Cursor_hasAttrs_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_hasAttrs(C);
    return Napi::Number::New(env, result);
}

// If cursor refers to a variable declaration that has external storage
// returns 1. If cursor refers to a variable declaration that doesn't have
// external storage returns 0. Otherwise returns -1.
static Napi::Value clang_Cursor_hasVarDeclExternalStorage_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_hasVarDeclExternalStorage(cursor);
    return Napi::Number::New(env, result);
}

// If cursor refers to a variable declaration that has global storage returns 1.
// If cursor refers to a variable declaration that doesn't have global storage
// returns 0. Otherwise returns -1.
static Napi::Value clang_Cursor_hasVarDeclGlobalStorage_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_hasVarDeclGlobalStorage(cursor);
    return Napi::Number::New(env, result);
}

// Determine whether the given cursor represents an anonymous
// tag or namespace
static Napi::Value clang_Cursor_isAnonymous_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_isAnonymous(C);
    return Napi::Number::New(env, result);
}

// Determine whether the given cursor represents an anonymous record
// declaration.
static Napi::Value clang_Cursor_isAnonymousRecordDecl_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_isAnonymousRecordDecl(C);
    return Napi::Number::New(env, result);
}

// Returns non-zero if the cursor specifies a Record member that is a bit-field.
static Napi::Value clang_Cursor_isBitField_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_isBitField(C);
    return Napi::Number::New(env, result);
}

// Given a cursor pointing to a C++ method call or an Objective-C
// message, returns non-zero if the method/message is "dynamic", meaning:
// 
// For a C++ method: the call is virtual.
// For an Objective-C message: the receiver is an object instance, not 'super'
// or a specific class.
// 
// If the method/message is "static" or the cursor does not point to a
// method/message, it will return zero.
static Napi::Value clang_Cursor_isDynamicCall_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_isDynamicCall(C);
    return Napi::Number::New(env, result);
}

// Returns non-zero if the given cursor points to a symbol marked with
// external_source_symbol attribute.
// 
// \param language If non-NULL, and the attribute is present, will be set to
// the 'language' string from the attribute.
// 
// \param definedIn If non-NULL, and the attribute is present, will be set to
// the 'definedIn' string from the attribute.
// 
// \param isGenerated If non-NULL, and the attribute is present, will be set to
// non-zero if the 'generated_declaration' is set in the attribute.
static Napi::Value clang_Cursor_isExternalSymbol_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 4) {
        Napi::TypeError::New(env, "Expected 4 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: language (CXString *)
    CXString * language = nullptr;
    if (!info[1].IsNull() && !info[1].IsUndefined() && info[1].IsObject()) {
        language = static_cast<CXString *>(unwrapPointer(info[1].As<Napi::Object>()));
    }
    // Parameter: definedIn (CXString *)
    CXString * definedIn = nullptr;
    if (!info[2].IsNull() && !info[2].IsUndefined() && info[2].IsObject()) {
        definedIn = static_cast<CXString *>(unwrapPointer(info[2].As<Napi::Object>()));
    }
    // Parameter: isGenerated (unsigned int *)
    unsigned int * isGenerated = *static_cast<unsigned int **>(unwrapPointer(info[3].As<Napi::Object>()));
    
    auto result = clang_Cursor_isExternalSymbol(C, language, definedIn, isGenerated);
    return Napi::Number::New(env, result);
}

// Determine whether a  CXCursor that is a function declaration, is an
// inline declaration.
static Napi::Value clang_Cursor_isFunctionInlined_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_isFunctionInlined(C);
    return Napi::Number::New(env, result);
}

// Determine whether the given cursor represents an inline namespace
// declaration.
static Napi::Value clang_Cursor_isInlineNamespace_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_isInlineNamespace(C);
    return Napi::Number::New(env, result);
}

// Determine whether a  CXCursor that is a macro, is a
// builtin one.
static Napi::Value clang_Cursor_isMacroBuiltin_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_isMacroBuiltin(C);
    return Napi::Number::New(env, result);
}

// Determine whether a  CXCursor that is a macro, is
// function like.
static Napi::Value clang_Cursor_isMacroFunctionLike_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_isMacroFunctionLike(C);
    return Napi::Number::New(env, result);
}

// Returns non-zero if \p cursor is null.
static Napi::Value clang_Cursor_isNull_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_isNull(cursor);
    return Napi::Number::New(env, result);
}

// Given a cursor that represents an Objective-C method or property
// declaration, return non-zero if the declaration was affected by "\@optional".
// Returns zero if the cursor is not such a declaration or it is "\@required".
static Napi::Value clang_Cursor_isObjCOptional_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_isObjCOptional(C);
    return Napi::Number::New(env, result);
}

// Returns non-zero if the given cursor is a variadic function or method.
static Napi::Value clang_Cursor_isVariadic_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Cursor_isVariadic(C);
    return Napi::Number::New(env, result);
}

// Queries a CXCursorSet to see if it contains a specific CXCursor.
// 
// \returns non-zero if the set contains the specified cursor.
static Napi::Value clang_CXCursorSet_contains_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cset (CXCursorSet)
    CXCursorSet cset = *static_cast<CXCursorSet*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    auto result = clang_CXCursorSet_contains(cset, cursor);
    return Napi::Number::New(env, result);
}

// Inserts a CXCursor into a CXCursorSet.
// 
// \returns zero if the CXCursor was already in the set, and non-zero otherwise.
static Napi::Value clang_CXCursorSet_insert_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cset (CXCursorSet)
    CXCursorSet cset = *static_cast<CXCursorSet*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    auto result = clang_CXCursorSet_insert(cset, cursor);
    return Napi::Number::New(env, result);
}

// Gets the general options associated with a CXIndex.
// 
// This function allows to obtain the final option values used by libclang after
// specifying the option policies via CXChoice enumerators.
// 
// \returns A bitmask of options, a bitwise OR of CXGlobalOpt_XXX flags that
// are associated with the given CXIndex object.
static Napi::Value clang_CXIndex_getGlobalOptions_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXIndex)
    CXIndex arg0 = *static_cast<CXIndex*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_CXIndex_getGlobalOptions(arg0);
    return Napi::Number::New(env, result);
}

// Sets general options associated with a CXIndex.
// 
// This function is DEPRECATED. Set
// CXIndexOptions::ThreadBackgroundPriorityForIndexing and/or
// CXIndexOptions::ThreadBackgroundPriorityForEditing and call
// clang_createIndexWithOptions() instead.
// 
// For example:
// \code
// CXIndex idx = ...;
// clang_CXIndex_setGlobalOptions(idx,
// clang_CXIndex_getGlobalOptions(idx) |
// CXGlobalOpt_ThreadBackgroundPriorityForIndexing);
// \endcode
// 
// \param options A bitmask of options, a bitwise OR of CXGlobalOpt_XXX flags.
static Napi::Value clang_CXIndex_setGlobalOptions_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXIndex)
    CXIndex arg0 = *static_cast<CXIndex*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: options (unsigned int)
    auto options = info[1].As<Napi::Number>().Uint32Value();
    
    clang_CXIndex_setGlobalOptions(arg0, options);
    return env.Undefined();
}

// Sets the invocation emission path option in a CXIndex.
// 
// This function is DEPRECATED. Set CXIndexOptions::InvocationEmissionPath and
// call clang_createIndexWithOptions() instead.
// 
// The invocation emission path specifies a path which will contain log
// files for certain libclang invocations. A null value (default) implies that
// libclang invocations are not logged..
static Napi::Value clang_CXIndex_setInvocationEmissionPathOption_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXIndex)
    CXIndex arg0 = *static_cast<CXIndex*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: Path (const char *)
    std::string Path_str = info[1].As<Napi::String>().Utf8Value();
    const char* Path = Path_str.c_str();
    
    clang_CXIndex_setInvocationEmissionPathOption(arg0, Path);
    return env.Undefined();
}

// Determine if a C++ constructor is a converting constructor.
static Napi::Value clang_CXXConstructor_isConvertingConstructor_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_CXXConstructor_isConvertingConstructor(C);
    return Napi::Number::New(env, result);
}

// Determine if a C++ constructor is a copy constructor.
static Napi::Value clang_CXXConstructor_isCopyConstructor_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_CXXConstructor_isCopyConstructor(C);
    return Napi::Number::New(env, result);
}

// Determine if a C++ constructor is the default constructor.
static Napi::Value clang_CXXConstructor_isDefaultConstructor_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_CXXConstructor_isDefaultConstructor(C);
    return Napi::Number::New(env, result);
}

// Determine if a C++ constructor is a move constructor.
static Napi::Value clang_CXXConstructor_isMoveConstructor_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_CXXConstructor_isMoveConstructor(C);
    return Napi::Number::New(env, result);
}

// Determine if a C++ field is declared 'mutable'.
static Napi::Value clang_CXXField_isMutable_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_CXXField_isMutable(C);
    return Napi::Number::New(env, result);
}

// Determine if a C++ member function or member function template is
// declared 'const'.
static Napi::Value clang_CXXMethod_isConst_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_CXXMethod_isConst(C);
    return Napi::Number::New(env, result);
}

// Determine if a C++ member function is a copy-assignment operator,
// returning 1 if such is the case and 0 otherwise.
// 
// > A copy-assignment operator `X::operator=` is a non-static,
// > non-template member function of _class_ `X` with exactly one
// > parameter of type `X`, `X&`, `const X&`, `volatile X&` or `const
// > volatile X&`.
// 
// That is, for example, the `operator=` in:
// 
// class Foo {
// bool operator=(const volatile Foo&);
// };
// 
// Is a copy-assignment operator, while the `operator=` in:
// 
// class Bar {
// bool operator=(const int&);
// };
// 
// Is not.
static Napi::Value clang_CXXMethod_isCopyAssignmentOperator_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_CXXMethod_isCopyAssignmentOperator(C);
    return Napi::Number::New(env, result);
}

// Determine if a C++ method is declared '= default'.
static Napi::Value clang_CXXMethod_isDefaulted_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_CXXMethod_isDefaulted(C);
    return Napi::Number::New(env, result);
}

// Determine if a C++ method is declared '= delete'.
static Napi::Value clang_CXXMethod_isDeleted_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_CXXMethod_isDeleted(C);
    return Napi::Number::New(env, result);
}

// Determines if a C++ constructor or conversion function was declared
// explicit, returning 1 if such is the case and 0 otherwise.
// 
// Constructors or conversion functions are declared explicit through
// the use of the explicit specifier.
// 
// For example, the following constructor and conversion function are
// not explicit as they lack the explicit specifier:
// 
// class Foo {
// Foo();
// operator int();
// };
// 
// While the following constructor and conversion function are
// explicit as they are declared with the explicit specifier.
// 
// class Foo {
// explicit Foo();
// explicit operator int();
// };
// 
// This function will return 0 when given a cursor pointing to one of
// the former declarations and it will return 1 for a cursor pointing
// to the latter declarations.
// 
// The explicit specifier allows the user to specify a
// conditional compile-time expression whose value decides
// whether the marked element is explicit or not.
// 
// For example:
// 
// constexpr bool foo(int i) { return i % 2 == 0; }
// 
// class Foo {
// explicit(foo(1)) Foo();
// explicit(foo(2)) operator int();
// }
// 
// This function will return 0 for the constructor and 1 for
// the conversion function.
static Napi::Value clang_CXXMethod_isExplicit_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_CXXMethod_isExplicit(C);
    return Napi::Number::New(env, result);
}

// Determine if a C++ member function is a move-assignment operator,
// returning 1 if such is the case and 0 otherwise.
// 
// > A move-assignment operator `X::operator=` is a non-static,
// > non-template member function of _class_ `X` with exactly one
// > parameter of type `X&&`, `const X&&`, `volatile X&&` or `const
// > volatile X&&`.
// 
// That is, for example, the `operator=` in:
// 
// class Foo {
// bool operator=(const volatile Foo&&);
// };
// 
// Is a move-assignment operator, while the `operator=` in:
// 
// class Bar {
// bool operator=(const int&&);
// };
// 
// Is not.
static Napi::Value clang_CXXMethod_isMoveAssignmentOperator_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_CXXMethod_isMoveAssignmentOperator(C);
    return Napi::Number::New(env, result);
}

// Determine if a C++ member function or member function template is
// pure virtual.
static Napi::Value clang_CXXMethod_isPureVirtual_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_CXXMethod_isPureVirtual(C);
    return Napi::Number::New(env, result);
}

// Determine if a C++ member function or member function template is
// declared 'static'.
static Napi::Value clang_CXXMethod_isStatic_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_CXXMethod_isStatic(C);
    return Napi::Number::New(env, result);
}

// Determine if a C++ member function or member function template is
// explicitly declared 'virtual' or if it overrides a virtual method from
// one of the base classes.
static Napi::Value clang_CXXMethod_isVirtual_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_CXXMethod_isVirtual(C);
    return Napi::Number::New(env, result);
}

// Determine if a C++ record is abstract, i.e. whether a class or struct
// has a pure virtual member function.
static Napi::Value clang_CXXRecord_isAbstract_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_CXXRecord_isAbstract(C);
    return Napi::Number::New(env, result);
}

// Returns a default set of code-completion options that can be
// passed to\c clang_codeCompleteAt().
static Napi::Value clang_defaultCodeCompleteOptions_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    
    auto result = clang_defaultCodeCompleteOptions();
    return Napi::Number::New(env, result);
}

// Retrieve the set of display options most similar to the
// default behavior of the clang compiler.
// 
// \returns A set of display options suitable for use with \c
// clang_formatDiagnostic().
static Napi::Value clang_defaultDiagnosticDisplayOptions_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    
    auto result = clang_defaultDiagnosticDisplayOptions();
    return Napi::Number::New(env, result);
}

// Returns the set of flags that is suitable for parsing a translation
// unit that is being edited.
// 
// The set of flags returned provide options for \c clang_parseTranslationUnit()
// to indicate that the translation unit is likely to be reparsed many times,
// either explicitly (via \c clang_reparseTranslationUnit()) or implicitly
// (e.g., by code completion (\c clang_codeCompletionAt())). The returned flag
// set contains an unspecified set of optimizations (e.g., the precompiled
// preamble) geared toward improving the performance of these routines. The
// set of optimizations enabled may change from one version to the next.
static Napi::Value clang_defaultEditingTranslationUnitOptions_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    
    auto result = clang_defaultEditingTranslationUnitOptions();
    return Napi::Number::New(env, result);
}

// Returns the set of flags that is suitable for reparsing a translation
// unit.
// 
// The set of flags returned provide options for
// \c clang_reparseTranslationUnit() by default. The returned flag
// set contains an unspecified set of optimizations geared toward common uses
// of reparsing. The set of optimizations enabled may change from one version
// to the next.
static Napi::Value clang_defaultReparseOptions_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: TU (CXTranslationUnit)
    CXTranslationUnit TU = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_defaultReparseOptions(TU);
    return Napi::Number::New(env, result);
}

// Returns the set of flags that is suitable for saving a translation
// unit.
// 
// The set of flags returned provide options for
// \c clang_saveTranslationUnit() by default. The returned flag
// set contains an unspecified set of options that save translation units with
// the most commonly-requested data.
static Napi::Value clang_defaultSaveOptions_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: TU (CXTranslationUnit)
    CXTranslationUnit TU = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_defaultSaveOptions(TU);
    return Napi::Number::New(env, result);
}

// Free the given set of code-completion results.
static Napi::Value clang_disposeCodeCompleteResults_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Results (CXCodeCompleteResults *)
    CXCodeCompleteResults * Results = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        Results = static_cast<CXCodeCompleteResults *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    
    clang_disposeCodeCompleteResults(Results);
    return env.Undefined();
}

// Disposes a CXCursorSet and releases its associated memory.
static Napi::Value clang_disposeCXCursorSet_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cset (CXCursorSet)
    CXCursorSet cset = *static_cast<CXCursorSet*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    clang_disposeCXCursorSet(cset);
    return env.Undefined();
}

// Free the memory associated with a \c CXPlatformAvailability structure.
static Napi::Value clang_disposeCXPlatformAvailability_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: availability (CXPlatformAvailability *)
    CXPlatformAvailability * availability = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        availability = static_cast<CXPlatformAvailability *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    
    clang_disposeCXPlatformAvailability(availability);
    return env.Undefined();
}

static Napi::Value clang_disposeCXTUResourceUsage_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: usage (CXTUResourceUsage)
    CXTUResourceUsage usage = *static_cast<CXTUResourceUsage*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    clang_disposeCXTUResourceUsage(usage);
    return env.Undefined();
}

// Destroy a diagnostic.
static Napi::Value clang_disposeDiagnostic_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Diagnostic (CXDiagnostic)
    CXDiagnostic Diagnostic = *static_cast<CXDiagnostic*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    clang_disposeDiagnostic(Diagnostic);
    return env.Undefined();
}

// Release a CXDiagnosticSet and all of its contained diagnostics.
static Napi::Value clang_disposeDiagnosticSet_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Diags (CXDiagnosticSet)
    CXDiagnosticSet Diags = *static_cast<CXDiagnosticSet*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    clang_disposeDiagnosticSet(Diags);
    return env.Undefined();
}

// Destroy the given index.
// 
// The index must not be destroyed until all of the translation units created
// within that index have been destroyed.
static Napi::Value clang_disposeIndex_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: index (CXIndex)
    CXIndex index = *static_cast<CXIndex*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    clang_disposeIndex(index);
    return env.Undefined();
}

// Free the set of overridden cursors returned by \c
// clang_getOverriddenCursors().
static Napi::Value clang_disposeOverriddenCursors_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: overridden (CXCursor *)
    CXCursor * overridden = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        overridden = static_cast<CXCursor *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    
    clang_disposeOverriddenCursors(overridden);
    return env.Undefined();
}

// Destroy the given \c CXSourceRangeList.
static Napi::Value clang_disposeSourceRangeList_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: ranges (CXSourceRangeList *)
    CXSourceRangeList * ranges = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        ranges = static_cast<CXSourceRangeList *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    
    clang_disposeSourceRangeList(ranges);
    return env.Undefined();
}

// Free the given string.
static Napi::Value clang_disposeString_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: string (CXString)
    CXString string = *static_cast<CXString*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    clang_disposeString(string);
    return env.Undefined();
}

// Free the given string set.
static Napi::Value clang_disposeStringSet_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: set (CXStringSet *)
    CXStringSet * set = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        set = static_cast<CXStringSet *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    
    clang_disposeStringSet(set);
    return env.Undefined();
}

// Free the given set of tokens.
static Napi::Value clang_disposeTokens_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: TU (CXTranslationUnit)
    CXTranslationUnit TU = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: Tokens (CXToken *)
    CXToken * Tokens = nullptr;
    if (!info[1].IsNull() && !info[1].IsUndefined() && info[1].IsObject()) {
        Tokens = static_cast<CXToken *>(unwrapPointer(info[1].As<Napi::Object>()));
    }
    // Parameter: NumTokens (unsigned int)
    auto NumTokens = info[2].As<Napi::Number>().Uint32Value();
    
    clang_disposeTokens(TU, Tokens, NumTokens);
    return env.Undefined();
}

// Destroy the specified CXTranslationUnit object.
static Napi::Value clang_disposeTranslationUnit_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXTranslationUnit)
    CXTranslationUnit arg0 = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    clang_disposeTranslationUnit(arg0);
    return env.Undefined();
}

static Napi::Value clang_enableStackTraces_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    
    clang_enableStackTraces();
    return env.Undefined();
}

// Determine if an enum declaration refers to a scoped enum.
static Napi::Value clang_EnumDecl_isScoped_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_EnumDecl_isScoped(C);
    return Napi::Number::New(env, result);
}

// Determine whether two cursors are equivalent.
static Napi::Value clang_equalCursors_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: arg1 (CXCursor)
    CXCursor arg1 = *static_cast<CXCursor*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    auto result = clang_equalCursors(arg0, arg1);
    return Napi::Number::New(env, result);
}

// Determine whether two source locations, which must refer into
// the same translation unit, refer to exactly the same point in the source
// code.
// 
// \returns non-zero if the source locations refer to the same location, zero
// if they refer to different locations.
static Napi::Value clang_equalLocations_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: loc1 (CXSourceLocation)
    CXSourceLocation loc1 = *static_cast<CXSourceLocation*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: loc2 (CXSourceLocation)
    CXSourceLocation loc2 = *static_cast<CXSourceLocation*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    auto result = clang_equalLocations(loc1, loc2);
    return Napi::Number::New(env, result);
}

// Determine whether two ranges are equivalent.
// 
// \returns non-zero if the ranges are the same, zero if they differ.
static Napi::Value clang_equalRanges_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: range1 (CXSourceRange)
    CXSourceRange range1 = *static_cast<CXSourceRange*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: range2 (CXSourceRange)
    CXSourceRange range2 = *static_cast<CXSourceRange*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    auto result = clang_equalRanges(range1, range2);
    return Napi::Number::New(env, result);
}

// Determine whether two CXTypes represent the same type.
// 
// \returns non-zero if the CXTypes represent the same type and
// zero otherwise.
static Napi::Value clang_equalTypes_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: A (CXType)
    CXType A = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: B (CXType)
    CXType B = *static_cast<CXType*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    auto result = clang_equalTypes(A, B);
    return Napi::Number::New(env, result);
}

// Disposes the created Eval memory.
static Napi::Value clang_EvalResult_dispose_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: E (CXEvalResult)
    CXEvalResult E = *static_cast<CXEvalResult*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    clang_EvalResult_dispose(E);
    return env.Undefined();
}

// Returns the evaluation result as double if the
// kind is double.
static Napi::Value clang_EvalResult_getAsDouble_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: E (CXEvalResult)
    CXEvalResult E = *static_cast<CXEvalResult*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_EvalResult_getAsDouble(E);
    return Napi::Number::New(env, result);
}

// Returns the evaluation result as integer if the
// kind is Int.
static Napi::Value clang_EvalResult_getAsInt_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: E (CXEvalResult)
    CXEvalResult E = *static_cast<CXEvalResult*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_EvalResult_getAsInt(E);
    return Napi::Number::New(env, result);
}

// Returns the evaluation result as a long long integer if the
// kind is Int. This prevents overflows that may happen if the result is
// returned with clang_EvalResult_getAsInt.
static Napi::Value clang_EvalResult_getAsLongLong_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: E (CXEvalResult)
    CXEvalResult E = *static_cast<CXEvalResult*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_EvalResult_getAsLongLong(E);
    return Napi::BigInt::New(env, result);
}

// Returns the evaluation result as a constant string if the
// kind is other than Int or float. User must not free this pointer,
// instead call clang_EvalResult_dispose on the CXEvalResult returned
// by clang_Cursor_Evaluate.
static Napi::Value clang_EvalResult_getAsStr_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: E (CXEvalResult)
    CXEvalResult E = *static_cast<CXEvalResult*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_EvalResult_getAsStr(E);
    if (result == nullptr) {
        return env.Null();
    }
    return Napi::String::New(env, result);
}

// Returns the evaluation result as an unsigned integer if
// the kind is Int and clang_EvalResult_isUnsignedInt is non-zero.
static Napi::Value clang_EvalResult_getAsUnsigned_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: E (CXEvalResult)
    CXEvalResult E = *static_cast<CXEvalResult*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_EvalResult_getAsUnsigned(E);
    return Napi::BigInt::New(env, result);
}

// Returns the kind of the evaluated result.
static Napi::Value clang_EvalResult_getKind_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: E (CXEvalResult)
    CXEvalResult E = *static_cast<CXEvalResult*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_EvalResult_getKind(E);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Returns a non-zero value if the kind is Int and the evaluation
// result resulted in an unsigned integer.
static Napi::Value clang_EvalResult_isUnsignedInt_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: E (CXEvalResult)
    CXEvalResult E = *static_cast<CXEvalResult*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_EvalResult_isUnsignedInt(E);
    return Napi::Number::New(env, result);
}

static Napi::Value clang_executeOnThread_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: fn (void (*)(void *))
    // Parameter: fn (void (*)(void *))
    void (*fn)(void *) = nullptr;
    if (info[0].IsExternal()) {
        fn = reinterpret_cast<void (*)(void *)>(info[0].As<Napi::External<void>>().Data());
    }
    // Parameter: user_data (void *)
    void * user_data = *static_cast<void **>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: stack_size (unsigned int)
    auto stack_size = info[2].As<Napi::Number>().Uint32Value();
    
    clang_executeOnThread(fn, user_data, stack_size);
    return env.Undefined();
}

// Returns non-zero if the \c file1 and \c file2 point to the same file,
// or they are both NULL.
static Napi::Value clang_File_isEqual_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: file1 (CXFile)
    CXFile file1 = *static_cast<CXFile*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: file2 (CXFile)
    CXFile file2 = *static_cast<CXFile*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    auto result = clang_File_isEqual(file1, file2);
    return Napi::Number::New(env, result);
}

// Returns the real path name of \c file.
// 
// An empty string may be returned. Use \c clang_getFileName() in that case.
static Napi::Value clang_File_tryGetRealPathName_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: file (CXFile)
    CXFile file = *static_cast<CXFile*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_File_tryGetRealPathName(file);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Find #import/#include directives in a specific file.
// 
// \param TU translation unit containing the file to query.
// 
// \param file to search for #import/#include directives.
// 
// \param visitor callback that will receive pairs of CXCursor/CXSourceRange for
// each directive found.
// 
// \returns one of the CXResult enumerators.
static Napi::Value clang_findIncludesInFile_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: TU (CXTranslationUnit)
    CXTranslationUnit TU = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: file (CXFile)
    CXFile file = *static_cast<CXFile*>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: visitor (CXCursorAndRangeVisitor)
    CXCursorAndRangeVisitor visitor = *static_cast<CXCursorAndRangeVisitor*>(unwrapPointer(info[2].As<Napi::Object>()));
    
    auto result = clang_findIncludesInFile(TU, file, visitor);
    return Napi::Number::New(env, static_cast<int>(result));
}

static Napi::Value clang_findIncludesInFileWithBlock_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXTranslationUnit)
    CXTranslationUnit arg0 = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: arg1 (CXFile)
    CXFile arg1 = *static_cast<CXFile*>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: arg2 (CXCursorAndRangeVisitorBlock)
    CXCursorAndRangeVisitorBlock arg2 = *static_cast<CXCursorAndRangeVisitorBlock*>(unwrapPointer(info[2].As<Napi::Object>()));
    
    auto result = clang_findIncludesInFileWithBlock(arg0, arg1, arg2);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Find references of a declaration in a specific file.
// 
// \param cursor pointing to a declaration or a reference of one.
// 
// \param file to search for references.
// 
// \param visitor callback that will receive pairs of CXCursor/CXSourceRange for
// each reference found.
// The CXSourceRange will point inside the file; if the reference is inside
// a macro (and not a macro argument) the CXSourceRange will be invalid.
// 
// \returns one of the CXResult enumerators.
static Napi::Value clang_findReferencesInFile_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: file (CXFile)
    CXFile file = *static_cast<CXFile*>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: visitor (CXCursorAndRangeVisitor)
    CXCursorAndRangeVisitor visitor = *static_cast<CXCursorAndRangeVisitor*>(unwrapPointer(info[2].As<Napi::Object>()));
    
    auto result = clang_findReferencesInFile(cursor, file, visitor);
    return Napi::Number::New(env, static_cast<int>(result));
}

static Napi::Value clang_findReferencesInFileWithBlock_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: arg1 (CXFile)
    CXFile arg1 = *static_cast<CXFile*>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: arg2 (CXCursorAndRangeVisitorBlock)
    CXCursorAndRangeVisitorBlock arg2 = *static_cast<CXCursorAndRangeVisitorBlock*>(unwrapPointer(info[2].As<Napi::Object>()));
    
    auto result = clang_findReferencesInFileWithBlock(arg0, arg1, arg2);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Format the given diagnostic in a manner that is suitable for display.
// 
// This routine will format the given diagnostic to a string, rendering
// the diagnostic according to the various options given. The
// \c clang_defaultDiagnosticDisplayOptions() function returns the set of
// options that most closely mimics the behavior of the clang compiler.
// 
// \param Diagnostic The diagnostic to print.
// 
// \param Options A set of options that control the diagnostic display,
// created by combining \c CXDiagnosticDisplayOptions values.
// 
// \returns A new string containing for formatted diagnostic.
static Napi::Value clang_formatDiagnostic_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Diagnostic (CXDiagnostic)
    CXDiagnostic Diagnostic = *static_cast<CXDiagnostic*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: Options (unsigned int)
    auto Options = info[1].As<Napi::Number>().Uint32Value();
    
    CXString result = clang_formatDiagnostic(Diagnostic, Options);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// free memory allocated by libclang, such as the buffer returned by
// \c CXVirtualFileOverlay() or \c clang_ModuleMapDescriptor_writeToBuffer().
// 
// \param buffer memory pointer to free.
static Napi::Value clang_free_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: buffer (void *)
    void * buffer = *static_cast<void **>(unwrapPointer(info[0].As<Napi::Object>()));
    
    clang_free(buffer);
    return env.Undefined();
}

// Returns the address space of the given type.
static Napi::Value clang_getAddressSpace_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getAddressSpace(T);
    return Napi::Number::New(env, result);
}

// Retrieve all ranges from all files that were skipped by the
// preprocessor.
// 
// The preprocessor will skip lines when they are surrounded by an
// if/ifdef/ifndef directive whose condition does not evaluate to true.
static Napi::Value clang_getAllSkippedRanges_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: tu (CXTranslationUnit)
    CXTranslationUnit tu = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXSourceRangeList * result = clang_getAllSkippedRanges(tu);
    CXSourceRangeList ** resultPtr = new CXSourceRangeList *;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXSourceRangeList *");
}

// Retrieve the type of a parameter of a function type.
// 
// If a non-function type is passed in or the function does not have enough
// parameters, an invalid type is returned.
static Napi::Value clang_getArgType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: i (unsigned int)
    auto i = info[1].As<Napi::Number>().Uint32Value();
    
    CXType result = clang_getArgType(T, i);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Return the element type of an array type.
// 
// If a non-array type is passed in, an invalid type is returned.
static Napi::Value clang_getArrayElementType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXType result = clang_getArrayElementType(T);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Return the array size of a constant array.
// 
// If a non-array type is passed in, -1 is returned.
static Napi::Value clang_getArraySize_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getArraySize(T);
    return Napi::BigInt::New(env, result);
}

// Retrieve the spelling of a given CXBinaryOperatorKind.
static Napi::Value clang_getBinaryOperatorKindSpelling_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: kind (enum CXBinaryOperatorKind)
    enum CXBinaryOperatorKind kind = static_cast<enum CXBinaryOperatorKind>(info[0].As<Napi::Number>().Int32Value());
    
    CXString result = clang_getBinaryOperatorKindSpelling(kind);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Return the timestamp for use with Clang's
// \c -fbuild-session-timestamp= option.
static Napi::Value clang_getBuildSessionTimestamp_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    
    auto result = clang_getBuildSessionTimestamp();
    return Napi::BigInt::New(env, result);
}

// Retrieve the canonical cursor corresponding to the given cursor.
// 
// In the C family of languages, many kinds of entities can be declared several
// times within a single translation unit. For example, a structure type can
// be forward-declared (possibly multiple times) and later defined:
// 
// \code
// struct X;
// struct X;
// struct X {
// int member;
// };
// \endcode
// 
// The declarations and the definition of \c X are represented by three
// different cursors, all of which are declarations of the same underlying
// entity. One of these cursor is considered the "canonical" cursor, which
// is effectively the representative for the underlying entity. One can
// determine if two cursors are declarations of the same underlying entity by
// comparing their canonical cursors.
// 
// \returns The canonical cursor for the entity referred to by the given cursor.
static Napi::Value clang_getCanonicalCursor_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXCursor result = clang_getCanonicalCursor(arg0);
    CXCursor* resultPtr = new CXCursor(result);
    return wrapOwnedPointer(env, resultPtr, "CXCursor");
}

// Return the canonical type for a CXType.
// 
// Clang's type system explicitly models typedefs and all the ways
// a specific type can be represented.  The canonical type is the underlying
// type with all the "sugar" removed.  For example, if 'T' is a typedef
// for 'int', the canonical type for 'T' would be 'int'.
static Napi::Value clang_getCanonicalType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXType result = clang_getCanonicalType(T);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Retrieve the child diagnostics of a CXDiagnostic.
// 
// This CXDiagnosticSet does not need to be released by
// clang_disposeDiagnosticSet.
static Napi::Value clang_getChildDiagnostics_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: D (CXDiagnostic)
    CXDiagnostic D = *static_cast<CXDiagnostic*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXDiagnosticSet result = clang_getChildDiagnostics(D);
    CXDiagnosticSet* resultPtr = new CXDiagnosticSet;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXDiagnosticSet");
}

// Return a version string, suitable for showing to a user, but not
// intended to be parsed (the format is not guaranteed to be stable).
static Napi::Value clang_getClangVersion_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    
    CXString result = clang_getClangVersion();
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Retrieve the annotation associated with the given completion string.
// 
// \param completion_string the completion string to query.
// 
// \param annotation_number the 0-based index of the annotation of the
// completion string.
// 
// \returns annotation string associated with the completion at index
// \c annotation_number, or a NULL string if that annotation is not available.
static Napi::Value clang_getCompletionAnnotation_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: completion_string (CXCompletionString)
    CXCompletionString completion_string = *static_cast<CXCompletionString*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: annotation_number (unsigned int)
    auto annotation_number = info[1].As<Napi::Number>().Uint32Value();
    
    CXString result = clang_getCompletionAnnotation(completion_string, annotation_number);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Determine the availability of the entity that this code-completion
// string refers to.
// 
// \param completion_string The completion string to query.
// 
// \returns The availability of the completion string.
static Napi::Value clang_getCompletionAvailability_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: completion_string (CXCompletionString)
    CXCompletionString completion_string = *static_cast<CXCompletionString*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getCompletionAvailability(completion_string);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Retrieve the brief documentation comment attached to the declaration
// that corresponds to the given completion string.
static Napi::Value clang_getCompletionBriefComment_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: completion_string (CXCompletionString)
    CXCompletionString completion_string = *static_cast<CXCompletionString*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_getCompletionBriefComment(completion_string);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Retrieve the completion string associated with a particular chunk
// within a completion string.
// 
// \param completion_string the completion string to query.
// 
// \param chunk_number the 0-based index of the chunk in the completion string.
// 
// \returns the completion string associated with the chunk at index
// \c chunk_number.
static Napi::Value clang_getCompletionChunkCompletionString_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: completion_string (CXCompletionString)
    CXCompletionString completion_string = *static_cast<CXCompletionString*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: chunk_number (unsigned int)
    auto chunk_number = info[1].As<Napi::Number>().Uint32Value();
    
    CXCompletionString result = clang_getCompletionChunkCompletionString(completion_string, chunk_number);
    CXCompletionString* resultPtr = new CXCompletionString;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXCompletionString");
}

// Determine the kind of a particular chunk within a completion string.
// 
// \param completion_string the completion string to query.
// 
// \param chunk_number the 0-based index of the chunk in the completion string.
// 
// \returns the kind of the chunk at the index \c chunk_number.
static Napi::Value clang_getCompletionChunkKind_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: completion_string (CXCompletionString)
    CXCompletionString completion_string = *static_cast<CXCompletionString*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: chunk_number (unsigned int)
    auto chunk_number = info[1].As<Napi::Number>().Uint32Value();
    
    auto result = clang_getCompletionChunkKind(completion_string, chunk_number);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Retrieve the text associated with a particular chunk within a
// completion string.
// 
// \param completion_string the completion string to query.
// 
// \param chunk_number the 0-based index of the chunk in the completion string.
// 
// \returns the text associated with the chunk at index \c chunk_number.
static Napi::Value clang_getCompletionChunkText_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: completion_string (CXCompletionString)
    CXCompletionString completion_string = *static_cast<CXCompletionString*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: chunk_number (unsigned int)
    auto chunk_number = info[1].As<Napi::Number>().Uint32Value();
    
    CXString result = clang_getCompletionChunkText(completion_string, chunk_number);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Fix-its that *must* be applied before inserting the text for the
// corresponding completion.
// 
// By default, clang_codeCompleteAt() only returns completions with empty
// fix-its. Extra completions with non-empty fix-its should be explicitly
// requested by setting CXCodeComplete_IncludeCompletionsWithFixIts.
// 
// For the clients to be able to compute position of the cursor after applying
// fix-its, the following conditions are guaranteed to hold for
// replacement_range of the stored fix-its:
// - Ranges in the fix-its are guaranteed to never contain the completion
// point (or identifier under completion point, if any) inside them, except
// at the start or at the end of the range.
// - If a fix-it range starts or ends with completion point (or starts or
// ends after the identifier under completion point), it will contain at
// least one character. It allows to unambiguously recompute completion
// point after applying the fix-it.
// 
// The intuition is that provided fix-its change code around the identifier we
// complete, but are not allowed to touch the identifier itself or the
// completion point. One example of completions with corrections are the ones
// replacing '.' with '->' and vice versa:
// 
// std::unique_ptr<std::vector<int>> vec_ptr;
// In 'vec_ptr.^', one of the completions is 'push_back', it requires
// replacing '.' with '->'.
// In 'vec_ptr->^', one of the completions is 'release', it requires
// replacing '->' with '.'.
// 
// \param results The structure keeping all completion results
// 
// \param completion_index The index of the completion
// 
// \param fixit_index The index of the fix-it for the completion at
// completion_index
// 
// \param replacement_range The fix-it range that must be replaced before the
// completion at completion_index can be applied
// 
// \returns The fix-it string that must replace the code at replacement_range
// before the completion at completion_index can be applied
static Napi::Value clang_getCompletionFixIt_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 4) {
        Napi::TypeError::New(env, "Expected 4 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: results (CXCodeCompleteResults *)
    CXCodeCompleteResults * results = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        results = static_cast<CXCodeCompleteResults *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    // Parameter: completion_index (unsigned int)
    auto completion_index = info[1].As<Napi::Number>().Uint32Value();
    // Parameter: fixit_index (unsigned int)
    auto fixit_index = info[2].As<Napi::Number>().Uint32Value();
    // Parameter: replacement_range (CXSourceRange *)
    CXSourceRange * replacement_range = nullptr;
    if (!info[3].IsNull() && !info[3].IsUndefined() && info[3].IsObject()) {
        replacement_range = static_cast<CXSourceRange *>(unwrapPointer(info[3].As<Napi::Object>()));
    }
    
    CXString result = clang_getCompletionFixIt(results, completion_index, fixit_index, replacement_range);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Retrieve the number of annotations associated with the given
// completion string.
// 
// \param completion_string the completion string to query.
// 
// \returns the number of annotations associated with the given completion
// string.
static Napi::Value clang_getCompletionNumAnnotations_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: completion_string (CXCompletionString)
    CXCompletionString completion_string = *static_cast<CXCompletionString*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getCompletionNumAnnotations(completion_string);
    return Napi::Number::New(env, result);
}

// Retrieve the number of fix-its for the given completion index.
// 
// Calling this makes sense only if CXCodeComplete_IncludeCompletionsWithFixIts
// option was set.
// 
// \param results The structure keeping all completion results
// 
// \param completion_index The index of the completion
// 
// \return The number of fix-its which must be applied before the completion at
// completion_index can be applied
static Napi::Value clang_getCompletionNumFixIts_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: results (CXCodeCompleteResults *)
    CXCodeCompleteResults * results = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        results = static_cast<CXCodeCompleteResults *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    // Parameter: completion_index (unsigned int)
    auto completion_index = info[1].As<Napi::Number>().Uint32Value();
    
    auto result = clang_getCompletionNumFixIts(results, completion_index);
    return Napi::Number::New(env, result);
}

// Retrieve the parent context of the given completion string.
// 
// The parent context of a completion string is the semantic parent of
// the declaration (if any) that the code completion represents. For example,
// a code completion for an Objective-C method would have the method's class
// or protocol as its context.
// 
// \param completion_string The code completion string whose parent is
// being queried.
// 
// \param kind DEPRECATED: always set to CXCursor_NotImplemented if non-NULL.
// 
// \returns The name of the completion parent, e.g., "NSObject" if
// the completion string represents a method in the NSObject class.
static Napi::Value clang_getCompletionParent_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: completion_string (CXCompletionString)
    CXCompletionString completion_string = *static_cast<CXCompletionString*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: kind (enum CXCursorKind *)
    enum CXCursorKind * kind = *static_cast<enum CXCursorKind **>(unwrapPointer(info[1].As<Napi::Object>()));
    
    CXString result = clang_getCompletionParent(completion_string, kind);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Determine the priority of this code completion.
// 
// The priority of a code completion indicates how likely it is that this
// particular completion is the completion that the user will select. The
// priority is selected by various internal heuristics.
// 
// \param completion_string The completion string to query.
// 
// \returns The priority of this completion string. Smaller values indicate
// higher-priority (more likely) completions.
static Napi::Value clang_getCompletionPriority_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: completion_string (CXCompletionString)
    CXCompletionString completion_string = *static_cast<CXCompletionString*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getCompletionPriority(completion_string);
    return Napi::Number::New(env, result);
}

// Retrieve the character data associated with the given string.
// 
// The returned data is a reference and not owned by the user. This data
// is only valid while the `CXString` is valid. This function is similar
// to `std::string::c_str()`.
static Napi::Value clang_getCString_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: string (CXString)
    CXString string = *static_cast<CXString*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getCString(string);
    if (result == nullptr) {
        return env.Null();
    }
    return Napi::String::New(env, result);
}

// Map a source location to the cursor that describes the entity at that
// location in the source code.
// 
// clang_getCursor() maps an arbitrary source location within a translation
// unit down to the most specific cursor that describes the entity at that
// location. For example, given an expression \c x + y, invoking
// clang_getCursor() with a source location pointing to "x" will return the
// cursor for "x"; similarly for "y". If the cursor points anywhere between
// "x" or "y" (e.g., on the + or the whitespace around it), clang_getCursor()
// will return a cursor referring to the "+" expression.
// 
// \returns a cursor representing the entity at the given source location, or
// a NULL cursor if no such entity can be found.
static Napi::Value clang_getCursor_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXTranslationUnit)
    CXTranslationUnit arg0 = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: arg1 (CXSourceLocation)
    CXSourceLocation arg1 = *static_cast<CXSourceLocation*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    CXCursor result = clang_getCursor(arg0, arg1);
    CXCursor* resultPtr = new CXCursor(result);
    return wrapOwnedPointer(env, resultPtr, "CXCursor");
}

// Determine the availability of the entity that this cursor refers to,
// taking the current target platform into account.
// 
// \param cursor The cursor to query.
// 
// \returns The availability of the cursor.
static Napi::Value clang_getCursorAvailability_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getCursorAvailability(cursor);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Retrieve the binary operator kind of this cursor.
// 
// If this cursor is not a binary operator then returns Invalid.
static Napi::Value clang_getCursorBinaryOperatorKind_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getCursorBinaryOperatorKind(cursor);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Retrieve a completion string for an arbitrary declaration or macro
// definition cursor.
// 
// \param cursor The cursor to query.
// 
// \returns A non-context-sensitive completion string for declaration and macro
// definition cursors, or NULL for other kinds of cursors.
static Napi::Value clang_getCursorCompletionString_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXCompletionString result = clang_getCursorCompletionString(cursor);
    CXCompletionString* resultPtr = new CXCompletionString;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXCompletionString");
}

// For a cursor that is either a reference to or a declaration
// of some entity, retrieve a cursor that describes the definition of
// that entity.
// 
// Some entities can be declared multiple times within a translation
// unit, but only one of those declarations can also be a
// definition. For example, given:
// 
// \code
// int f(int, int);
// int g(int x, int y) { return f(x, y); }
// int f(int a, int b) { return a + b; }
// int f(int, int);
// \endcode
// 
// there are three declarations of the function "f", but only the
// second one is a definition. The clang_getCursorDefinition()
// function will take any cursor pointing to a declaration of "f"
// (the first or fourth lines of the example) or a cursor referenced
// that uses "f" (the call to "f' inside "g") and will return a
// declaration cursor pointing to the definition (the second "f"
// declaration).
// 
// If given a cursor for which there is no corresponding definition,
// e.g., because there is no definition of that entity within this
// translation unit, returns a NULL cursor.
static Napi::Value clang_getCursorDefinition_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXCursor result = clang_getCursorDefinition(arg0);
    CXCursor* resultPtr = new CXCursor(result);
    return wrapOwnedPointer(env, resultPtr, "CXCursor");
}

// Retrieve the display name for the entity referenced by this cursor.
// 
// The display name contains extra information that helps identify the cursor,
// such as the parameters of a function or template or the arguments of a
// class template specialization.
static Napi::Value clang_getCursorDisplayName_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_getCursorDisplayName(arg0);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Retrieve the exception specification type associated with a given cursor.
// This is a value of type CXCursor_ExceptionSpecificationKind.
// 
// This only returns a valid result if the cursor refers to a function or
// method.
static Napi::Value clang_getCursorExceptionSpecificationType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getCursorExceptionSpecificationType(C);
    return Napi::Number::New(env, result);
}

// Retrieve the physical extent of the source construct referenced by
// the given cursor.
// 
// The extent of a cursor starts with the file/line/column pointing at the
// first character within the source construct that the cursor refers to and
// ends with the last character within that source construct. For a
// declaration, the extent covers the declaration itself. For a reference,
// the extent covers the location of the reference (e.g., where the referenced
// entity was actually used).
static Napi::Value clang_getCursorExtent_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXSourceRange result = clang_getCursorExtent(arg0);
    CXSourceRange* resultPtr = new CXSourceRange(result);
    return wrapOwnedPointer(env, resultPtr, "CXSourceRange");
}

// Retrieve the kind of the given cursor.
static Napi::Value clang_getCursorKind_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getCursorKind(arg0);
    return Napi::Number::New(env, static_cast<int>(result));
}

// \defgroup CINDEX_DEBUG Debugging facilities
// 
// These routines are used for testing and debugging, only, and should not
// be relied upon.
// 
// @{
static Napi::Value clang_getCursorKindSpelling_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Kind (enum CXCursorKind)
    enum CXCursorKind Kind = static_cast<enum CXCursorKind>(info[0].As<Napi::Number>().Int32Value());
    
    CXString result = clang_getCursorKindSpelling(Kind);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Determine the "language" of the entity referred to by a given cursor.
static Napi::Value clang_getCursorLanguage_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getCursorLanguage(cursor);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Determine the lexical parent of the given cursor.
// 
// The lexical parent of a cursor is the cursor in which the given \p cursor
// was actually written. For many declarations, the lexical and semantic parents
// are equivalent (the semantic parent is returned by
// \c clang_getCursorSemanticParent()). They diverge when declarations or
// definitions are provided out-of-line. For example:
// 
// \code
// class C {
// void f();
// };
// 
// void C::f() { }
// \endcode
// 
// In the out-of-line definition of \c C::f, the semantic parent is
// the class \c C, of which this function is a member. The lexical parent is
// the place where the declaration actually occurs in the source code; in this
// case, the definition occurs in the translation unit. In general, the
// lexical parent for a given entity can change without affecting the semantics
// of the program, and the lexical parent of different declarations of the
// same entity may be different. Changing the semantic parent of a declaration,
// on the other hand, can have a major impact on semantics, and redeclarations
// of a particular entity should all have the same semantic context.
// 
// In the example above, both declarations of \c C::f have \c C as their
// semantic context, while the lexical context of the first \c C::f is \c C
// and the lexical context of the second \c C::f is the translation unit.
// 
// For declarations written in the global scope, the lexical parent is
// the translation unit.
static Napi::Value clang_getCursorLexicalParent_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXCursor result = clang_getCursorLexicalParent(cursor);
    CXCursor* resultPtr = new CXCursor(result);
    return wrapOwnedPointer(env, resultPtr, "CXCursor");
}

// Determine the linkage of the entity referred to by a given cursor.
static Napi::Value clang_getCursorLinkage_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getCursorLinkage(cursor);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Retrieve the physical location of the source constructor referenced
// by the given cursor.
// 
// The location of a declaration is typically the location of the name of that
// declaration, where the name of that declaration would occur if it is
// unnamed, or some keyword that introduces that particular declaration.
// The location of a reference is where that reference occurs within the
// source code.
static Napi::Value clang_getCursorLocation_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXSourceLocation result = clang_getCursorLocation(arg0);
    CXSourceLocation* resultPtr = new CXSourceLocation(result);
    return wrapOwnedPointer(env, resultPtr, "CXSourceLocation");
}

// Determine the availability of the entity that this cursor refers to
// on any platforms for which availability information is known.
// 
// \param cursor The cursor to query.
// 
// \param always_deprecated If non-NULL, will be set to indicate whether the
// entity is deprecated on all platforms.
// 
// \param deprecated_message If non-NULL, will be set to the message text
// provided along with the unconditional deprecation of this entity. The client
// is responsible for deallocating this string.
// 
// \param always_unavailable If non-NULL, will be set to indicate whether the
// entity is unavailable on all platforms.
// 
// \param unavailable_message If non-NULL, will be set to the message text
// provided along with the unconditional unavailability of this entity. The
// client is responsible for deallocating this string.
// 
// \param availability If non-NULL, an array of CXPlatformAvailability instances
// that will be populated with platform availability information, up to either
// the number of platforms for which availability information is available (as
// returned by this function) or \c availability_size, whichever is smaller.
// 
// \param availability_size The number of elements available in the
// \c availability array.
// 
// \returns The number of platforms (N) for which availability information is
// available (which is unrelated to \c availability_size).
// 
// Note that the client is responsible for calling
// \c clang_disposeCXPlatformAvailability to free each of the
// platform-availability structures returned. There are
// \c min(N, availability_size) such structures.
static Napi::Value clang_getCursorPlatformAvailability_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 7) {
        Napi::TypeError::New(env, "Expected 7 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: always_deprecated (int *)
    int * always_deprecated = static_cast<int *>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: deprecated_message (CXString *)
    CXString * deprecated_message = nullptr;
    if (!info[2].IsNull() && !info[2].IsUndefined() && info[2].IsObject()) {
        deprecated_message = static_cast<CXString *>(unwrapPointer(info[2].As<Napi::Object>()));
    }
    // Parameter: always_unavailable (int *)
    int * always_unavailable = static_cast<int *>(unwrapPointer(info[3].As<Napi::Object>()));
    // Parameter: unavailable_message (CXString *)
    CXString * unavailable_message = nullptr;
    if (!info[4].IsNull() && !info[4].IsUndefined() && info[4].IsObject()) {
        unavailable_message = static_cast<CXString *>(unwrapPointer(info[4].As<Napi::Object>()));
    }
    // Parameter: availability (CXPlatformAvailability *)
    CXPlatformAvailability * availability = nullptr;
    if (!info[5].IsNull() && !info[5].IsUndefined() && info[5].IsObject()) {
        availability = static_cast<CXPlatformAvailability *>(unwrapPointer(info[5].As<Napi::Object>()));
    }
    // Parameter: availability_size (int)
    auto availability_size = info[6].As<Napi::Number>().Int32Value();
    
    auto result = clang_getCursorPlatformAvailability(cursor, always_deprecated, deprecated_message, always_unavailable, unavailable_message, availability, availability_size);
    return Napi::Number::New(env, result);
}

// Pretty print declarations.
// 
// \param Cursor The cursor representing a declaration.
// 
// \param Policy The policy to control the entities being printed. If
// NULL, a default policy is used.
// 
// \returns The pretty printed declaration or the empty string for
// other cursors.
static Napi::Value clang_getCursorPrettyPrinted_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Cursor (CXCursor)
    CXCursor Cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: Policy (CXPrintingPolicy)
    CXPrintingPolicy Policy = *static_cast<CXPrintingPolicy*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    CXString result = clang_getCursorPrettyPrinted(Cursor, Policy);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Retrieve the default policy for the cursor.
// 
// The policy should be released after use with \c
// clang_PrintingPolicy_dispose.
static Napi::Value clang_getCursorPrintingPolicy_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXPrintingPolicy result = clang_getCursorPrintingPolicy(arg0);
    CXPrintingPolicy* resultPtr = new CXPrintingPolicy;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXPrintingPolicy");
}

// For a cursor that is a reference, retrieve a cursor representing the
// entity that it references.
// 
// Reference cursors refer to other entities in the AST. For example, an
// Objective-C superclass reference cursor refers to an Objective-C class.
// This function produces the cursor for the Objective-C class from the
// cursor for the superclass reference. If the input cursor is a declaration or
// definition, it returns that declaration or definition unchanged.
// Otherwise, returns the NULL cursor.
static Napi::Value clang_getCursorReferenced_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXCursor result = clang_getCursorReferenced(arg0);
    CXCursor* resultPtr = new CXCursor(result);
    return wrapOwnedPointer(env, resultPtr, "CXCursor");
}

// Given a cursor that references something else, return the source range
// covering that reference.
// 
// \param C A cursor pointing to a member reference, a declaration reference, or
// an operator call.
// \param NameFlags A bitset with three independent flags:
// CXNameRange_WantQualifier, CXNameRange_WantTemplateArgs, and
// CXNameRange_WantSinglePiece.
// \param PieceIndex For contiguous names or when passing the flag
// CXNameRange_WantSinglePiece, only one piece with index 0 is
// available. When the CXNameRange_WantSinglePiece flag is not passed for a
// non-contiguous names, this index can be used to retrieve the individual
// pieces of the name. See also CXNameRange_WantSinglePiece.
// 
// \returns The piece of the name pointed to by the given cursor. If there is no
// name, or if the PieceIndex is out-of-range, a null-cursor will be returned.
static Napi::Value clang_getCursorReferenceNameRange_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: NameFlags (unsigned int)
    auto NameFlags = info[1].As<Napi::Number>().Uint32Value();
    // Parameter: PieceIndex (unsigned int)
    auto PieceIndex = info[2].As<Napi::Number>().Uint32Value();
    
    CXSourceRange result = clang_getCursorReferenceNameRange(C, NameFlags, PieceIndex);
    CXSourceRange* resultPtr = new CXSourceRange(result);
    return wrapOwnedPointer(env, resultPtr, "CXSourceRange");
}

// Retrieve the return type associated with a given cursor.
// 
// This only returns a valid type if the cursor refers to a function or method.
static Napi::Value clang_getCursorResultType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXType result = clang_getCursorResultType(C);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Determine the semantic parent of the given cursor.
// 
// The semantic parent of a cursor is the cursor that semantically contains
// the given \p cursor. For many declarations, the lexical and semantic parents
// are equivalent (the lexical parent is returned by
// \c clang_getCursorLexicalParent()). They diverge when declarations or
// definitions are provided out-of-line. For example:
// 
// \code
// class C {
// void f();
// };
// 
// void C::f() { }
// \endcode
// 
// In the out-of-line definition of \c C::f, the semantic parent is
// the class \c C, of which this function is a member. The lexical parent is
// the place where the declaration actually occurs in the source code; in this
// case, the definition occurs in the translation unit. In general, the
// lexical parent for a given entity can change without affecting the semantics
// of the program, and the lexical parent of different declarations of the
// same entity may be different. Changing the semantic parent of a declaration,
// on the other hand, can have a major impact on semantics, and redeclarations
// of a particular entity should all have the same semantic context.
// 
// In the example above, both declarations of \c C::f have \c C as their
// semantic context, while the lexical context of the first \c C::f is \c C
// and the lexical context of the second \c C::f is the translation unit.
// 
// For global declarations, the semantic parent is the translation unit.
static Napi::Value clang_getCursorSemanticParent_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXCursor result = clang_getCursorSemanticParent(cursor);
    CXCursor* resultPtr = new CXCursor(result);
    return wrapOwnedPointer(env, resultPtr, "CXCursor");
}

// Retrieve a name for the entity referenced by this cursor.
static Napi::Value clang_getCursorSpelling_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_getCursorSpelling(arg0);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Determine the "thread-local storage (TLS) kind" of the declaration
// referred to by a cursor.
static Napi::Value clang_getCursorTLSKind_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getCursorTLSKind(cursor);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Retrieve the type of a CXCursor (if any).
static Napi::Value clang_getCursorType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXType result = clang_getCursorType(C);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Retrieve the unary operator kind of this cursor.
// 
// If this cursor is not a unary operator then returns Invalid.
static Napi::Value clang_getCursorUnaryOperatorKind_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getCursorUnaryOperatorKind(cursor);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Retrieve a Unified Symbol Resolution (USR) for the entity referenced
// by the given cursor.
// 
// A Unified Symbol Resolution (USR) is a string that identifies a particular
// entity (function, class, variable, etc.) within a program. USRs can be
// compared across translation units to determine, e.g., when references in
// one translation refer to an entity defined in another translation unit.
static Napi::Value clang_getCursorUSR_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_getCursorUSR(arg0);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Describe the visibility of the entity referred to by a cursor.
// 
// This returns the default visibility if not explicitly specified by
// a visibility attribute. The default visibility may be changed by
// commandline arguments.
// 
// \param cursor The cursor to query.
// 
// \returns The visibility of the cursor.
static Napi::Value clang_getCursorVisibility_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getCursorVisibility(cursor);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Return the memory usage of a translation unit.  This object
// should be released with clang_disposeCXTUResourceUsage().
static Napi::Value clang_getCXTUResourceUsage_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: TU (CXTranslationUnit)
    CXTranslationUnit TU = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXTUResourceUsage result = clang_getCXTUResourceUsage(TU);
    CXTUResourceUsage* resultPtr = new CXTUResourceUsage(result);
    return wrapOwnedPointer(env, resultPtr, "CXTUResourceUsage");
}

// Returns the access control level for the referenced object.
// 
// If the cursor refers to a C++ declaration, its access control level within
// its parent scope is returned. Otherwise, if the cursor refers to a base
// specifier or access specifier, the specifier itself is returned.
static Napi::Value clang_getCXXAccessSpecifier_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getCXXAccessSpecifier(arg0);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Returns the Objective-C type encoding for the specified declaration.
static Napi::Value clang_getDeclObjCTypeEncoding_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_getDeclObjCTypeEncoding(C);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

static Napi::Value clang_getDefinitionSpellingAndExtent_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 7) {
        Napi::TypeError::New(env, "Expected 7 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: startBuf (const char **)
    const char ** startBuf = *static_cast<const char ***>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: endBuf (const char **)
    const char ** endBuf = *static_cast<const char ***>(unwrapPointer(info[2].As<Napi::Object>()));
    // Parameter: startLine (unsigned int *)
    unsigned int * startLine = *static_cast<unsigned int **>(unwrapPointer(info[3].As<Napi::Object>()));
    // Parameter: startColumn (unsigned int *)
    unsigned int * startColumn = *static_cast<unsigned int **>(unwrapPointer(info[4].As<Napi::Object>()));
    // Parameter: endLine (unsigned int *)
    unsigned int * endLine = *static_cast<unsigned int **>(unwrapPointer(info[5].As<Napi::Object>()));
    // Parameter: endColumn (unsigned int *)
    unsigned int * endColumn = *static_cast<unsigned int **>(unwrapPointer(info[6].As<Napi::Object>()));
    
    clang_getDefinitionSpellingAndExtent(arg0, startBuf, endBuf, startLine, startColumn, endLine, endColumn);
    return env.Undefined();
}

// Retrieve a diagnostic associated with the given translation unit.
// 
// \param Unit the translation unit to query.
// \param Index the zero-based diagnostic number to retrieve.
// 
// \returns the requested diagnostic. This diagnostic must be freed
// via a call to \c clang_disposeDiagnostic().
static Napi::Value clang_getDiagnostic_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Unit (CXTranslationUnit)
    CXTranslationUnit Unit = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: Index (unsigned int)
    auto Index = info[1].As<Napi::Number>().Uint32Value();
    
    CXDiagnostic result = clang_getDiagnostic(Unit, Index);
    CXDiagnostic* resultPtr = new CXDiagnostic;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXDiagnostic");
}

// Retrieve the category number for this diagnostic.
// 
// Diagnostics can be categorized into groups along with other, related
// diagnostics (e.g., diagnostics under the same warning flag). This routine
// retrieves the category number for the given diagnostic.
// 
// \returns The number of the category that contains this diagnostic, or zero
// if this diagnostic is uncategorized.
static Napi::Value clang_getDiagnosticCategory_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXDiagnostic)
    CXDiagnostic arg0 = *static_cast<CXDiagnostic*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getDiagnosticCategory(arg0);
    return Napi::Number::New(env, result);
}

// Retrieve the name of a particular diagnostic category.  This
// is now deprecated.  Use clang_getDiagnosticCategoryText()
// instead.
// 
// \param Category A diagnostic category number, as returned by
// \c clang_getDiagnosticCategory().
// 
// \returns The name of the given diagnostic category.
static Napi::Value clang_getDiagnosticCategoryName_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Category (unsigned int)
    auto Category = info[0].As<Napi::Number>().Uint32Value();
    
    CXString result = clang_getDiagnosticCategoryName(Category);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Retrieve the diagnostic category text for a given diagnostic.
// 
// \returns The text of the given diagnostic category.
static Napi::Value clang_getDiagnosticCategoryText_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXDiagnostic)
    CXDiagnostic arg0 = *static_cast<CXDiagnostic*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_getDiagnosticCategoryText(arg0);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Retrieve the replacement information for a given fix-it.
// 
// Fix-its are described in terms of a source range whose contents
// should be replaced by a string. This approach generalizes over
// three kinds of operations: removal of source code (the range covers
// the code to be removed and the replacement string is empty),
// replacement of source code (the range covers the code to be
// replaced and the replacement string provides the new code), and
// insertion (both the start and end of the range point at the
// insertion location, and the replacement string provides the text to
// insert).
// 
// \param Diagnostic The diagnostic whose fix-its are being queried.
// 
// \param FixIt The zero-based index of the fix-it.
// 
// \param ReplacementRange The source range whose contents will be
// replaced with the returned replacement string. Note that source
// ranges are half-open ranges [a, b), so the source code should be
// replaced from a and up to (but not including) b.
// 
// \returns A string containing text that should be replace the source
// code indicated by the \c ReplacementRange.
static Napi::Value clang_getDiagnosticFixIt_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Diagnostic (CXDiagnostic)
    CXDiagnostic Diagnostic = *static_cast<CXDiagnostic*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: FixIt (unsigned int)
    auto FixIt = info[1].As<Napi::Number>().Uint32Value();
    // Parameter: ReplacementRange (CXSourceRange *)
    CXSourceRange * ReplacementRange = nullptr;
    if (!info[2].IsNull() && !info[2].IsUndefined() && info[2].IsObject()) {
        ReplacementRange = static_cast<CXSourceRange *>(unwrapPointer(info[2].As<Napi::Object>()));
    }
    
    CXString result = clang_getDiagnosticFixIt(Diagnostic, FixIt, ReplacementRange);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Retrieve a diagnostic associated with the given CXDiagnosticSet.
// 
// \param Diags the CXDiagnosticSet to query.
// \param Index the zero-based diagnostic number to retrieve.
// 
// \returns the requested diagnostic. This diagnostic must be freed
// via a call to \c clang_disposeDiagnostic().
static Napi::Value clang_getDiagnosticInSet_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Diags (CXDiagnosticSet)
    CXDiagnosticSet Diags = *static_cast<CXDiagnosticSet*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: Index (unsigned int)
    auto Index = info[1].As<Napi::Number>().Uint32Value();
    
    CXDiagnostic result = clang_getDiagnosticInSet(Diags, Index);
    CXDiagnostic* resultPtr = new CXDiagnostic;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXDiagnostic");
}

// Retrieve the source location of the given diagnostic.
// 
// This location is where Clang would print the caret ('^') when
// displaying the diagnostic on the command line.
static Napi::Value clang_getDiagnosticLocation_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXDiagnostic)
    CXDiagnostic arg0 = *static_cast<CXDiagnostic*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXSourceLocation result = clang_getDiagnosticLocation(arg0);
    CXSourceLocation* resultPtr = new CXSourceLocation(result);
    return wrapOwnedPointer(env, resultPtr, "CXSourceLocation");
}

// Determine the number of fix-it hints associated with the
// given diagnostic.
static Napi::Value clang_getDiagnosticNumFixIts_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Diagnostic (CXDiagnostic)
    CXDiagnostic Diagnostic = *static_cast<CXDiagnostic*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getDiagnosticNumFixIts(Diagnostic);
    return Napi::Number::New(env, result);
}

// Determine the number of source ranges associated with the given
// diagnostic.
static Napi::Value clang_getDiagnosticNumRanges_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXDiagnostic)
    CXDiagnostic arg0 = *static_cast<CXDiagnostic*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getDiagnosticNumRanges(arg0);
    return Napi::Number::New(env, result);
}

// Retrieve the name of the command-line option that enabled this
// diagnostic.
// 
// \param Diag The diagnostic to be queried.
// 
// \param Disable If non-NULL, will be set to the option that disables this
// diagnostic (if any).
// 
// \returns A string that contains the command-line option used to enable this
// warning, such as "-Wconversion" or "-pedantic".
static Napi::Value clang_getDiagnosticOption_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Diag (CXDiagnostic)
    CXDiagnostic Diag = *static_cast<CXDiagnostic*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: Disable (CXString *)
    CXString * Disable = nullptr;
    if (!info[1].IsNull() && !info[1].IsUndefined() && info[1].IsObject()) {
        Disable = static_cast<CXString *>(unwrapPointer(info[1].As<Napi::Object>()));
    }
    
    CXString result = clang_getDiagnosticOption(Diag, Disable);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Retrieve a source range associated with the diagnostic.
// 
// A diagnostic's source ranges highlight important elements in the source
// code. On the command line, Clang displays source ranges by
// underlining them with '~' characters.
// 
// \param Diagnostic the diagnostic whose range is being extracted.
// 
// \param Range the zero-based index specifying which range to
// 
// \returns the requested source range.
static Napi::Value clang_getDiagnosticRange_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Diagnostic (CXDiagnostic)
    CXDiagnostic Diagnostic = *static_cast<CXDiagnostic*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: Range (unsigned int)
    auto Range = info[1].As<Napi::Number>().Uint32Value();
    
    CXSourceRange result = clang_getDiagnosticRange(Diagnostic, Range);
    CXSourceRange* resultPtr = new CXSourceRange(result);
    return wrapOwnedPointer(env, resultPtr, "CXSourceRange");
}

// Retrieve the complete set of diagnostics associated with a
// translation unit.
// 
// \param Unit the translation unit to query.
static Napi::Value clang_getDiagnosticSetFromTU_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Unit (CXTranslationUnit)
    CXTranslationUnit Unit = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXDiagnosticSet result = clang_getDiagnosticSetFromTU(Unit);
    CXDiagnosticSet* resultPtr = new CXDiagnosticSet;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXDiagnosticSet");
}

// Determine the severity of the given diagnostic.
static Napi::Value clang_getDiagnosticSeverity_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXDiagnostic)
    CXDiagnostic arg0 = *static_cast<CXDiagnostic*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getDiagnosticSeverity(arg0);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Retrieve the text of the given diagnostic.
static Napi::Value clang_getDiagnosticSpelling_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXDiagnostic)
    CXDiagnostic arg0 = *static_cast<CXDiagnostic*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_getDiagnosticSpelling(arg0);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Return the element type of an array, complex, or vector type.
// 
// If a type is passed in that is not an array, complex, or vector type,
// an invalid type is returned.
static Napi::Value clang_getElementType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXType result = clang_getElementType(T);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Retrieve the integer value of an enum constant declaration as an unsigned
// long long.
// 
// If the cursor does not reference an enum constant declaration, ULLONG_MAX is
// returned. Since this is also potentially a valid constant value, the kind of
// the cursor must be verified before calling this function.
static Napi::Value clang_getEnumConstantDeclUnsignedValue_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getEnumConstantDeclUnsignedValue(C);
    return Napi::BigInt::New(env, result);
}

// Retrieve the integer value of an enum constant declaration as a signed
// long long.
// 
// If the cursor does not reference an enum constant declaration, LLONG_MIN is
// returned. Since this is also potentially a valid constant value, the kind of
// the cursor must be verified before calling this function.
static Napi::Value clang_getEnumConstantDeclValue_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getEnumConstantDeclValue(C);
    return Napi::BigInt::New(env, result);
}

// Retrieve the integer type of an enum declaration.
// 
// If the cursor does not reference an enum declaration, an invalid type is
// returned.
static Napi::Value clang_getEnumDeclIntegerType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXType result = clang_getEnumDeclIntegerType(C);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Retrieve the exception specification type associated with a function type.
// This is a value of type CXCursor_ExceptionSpecificationKind.
// 
// If a non-function type is passed in, an error code of -1 is returned.
static Napi::Value clang_getExceptionSpecificationType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getExceptionSpecificationType(T);
    return Napi::Number::New(env, result);
}

// Retrieve the file, line, column, and offset represented by
// the given source location.
// 
// If the location refers into a macro expansion, retrieves the
// location of the macro expansion.
// 
// \param location the location within a source file that will be decomposed
// into its parts.
// 
// \param file [out] if non-NULL, will be set to the file to which the given
// source location points.
// 
// \param line [out] if non-NULL, will be set to the line to which the given
// source location points.
// 
// \param column [out] if non-NULL, will be set to the column to which the given
// source location points.
// 
// \param offset [out] if non-NULL, will be set to the offset into the
// buffer to which the given source location points.
static Napi::Value clang_getExpansionLocation_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 5) {
        Napi::TypeError::New(env, "Expected 5 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: location (CXSourceLocation)
    CXSourceLocation location = *static_cast<CXSourceLocation*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: file (CXFile *)
    CXFile * file = *static_cast<CXFile **>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: line (unsigned int *)
    unsigned int * line = *static_cast<unsigned int **>(unwrapPointer(info[2].As<Napi::Object>()));
    // Parameter: column (unsigned int *)
    unsigned int * column = *static_cast<unsigned int **>(unwrapPointer(info[3].As<Napi::Object>()));
    // Parameter: offset (unsigned int *)
    unsigned int * offset = *static_cast<unsigned int **>(unwrapPointer(info[4].As<Napi::Object>()));
    
    clang_getExpansionLocation(location, file, line, column, offset);
    return env.Undefined();
}

// Retrieve the bit width of a bit-field declaration as an integer.
// 
// If the cursor does not reference a bit-field, or if the bit-field's width
// expression cannot be evaluated, -1 is returned.
// 
// For example:
// \code
// if (clang_Cursor_isBitField(Cursor)) {
// int Width = clang_getFieldDeclBitWidth(Cursor);
// if (Width != -1) {
// // The bit-field width is not value-dependent.
// }
// }
// \endcode
static Napi::Value clang_getFieldDeclBitWidth_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getFieldDeclBitWidth(C);
    return Napi::Number::New(env, result);
}

// Retrieve a file handle within the given translation unit.
// 
// \param tu the translation unit
// 
// \param file_name the name of the file.
// 
// \returns the file handle for the named file in the translation unit \p tu,
// or a NULL file handle if the file was not a part of this translation unit.
static Napi::Value clang_getFile_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: tu (CXTranslationUnit)
    CXTranslationUnit tu = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: file_name (const char *)
    std::string file_name_str = info[1].As<Napi::String>().Utf8Value();
    const char* file_name = file_name_str.c_str();
    
    CXFile result = clang_getFile(tu, file_name);
    CXFile* resultPtr = new CXFile;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXFile");
}

// Retrieve the buffer associated with the given file.
// 
// \param tu the translation unit
// 
// \param file the file for which to retrieve the buffer.
// 
// \param size [out] if non-NULL, will be set to the size of the buffer.
// 
// \returns a pointer to the buffer in memory that holds the contents of
// \p file, or a NULL pointer when the file is not loaded.
static Napi::Value clang_getFileContents_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: tu (CXTranslationUnit)
    CXTranslationUnit tu = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: file (CXFile)
    CXFile file = *static_cast<CXFile*>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: size (int *)
    size_t * size = static_cast<size_t *>(unwrapPointer(info[2].As<Napi::Object>()));
    
    auto result = clang_getFileContents(tu, file, size);
    if (result == nullptr) {
        return env.Null();
    }
    return Napi::String::New(env, result);
}

// Retrieve the file, line, column, and offset represented by
// the given source location.
// 
// If the location refers into a macro expansion, return where the macro was
// expanded or where the macro argument was written, if the location points at
// a macro argument.
// 
// \param location the location within a source file that will be decomposed
// into its parts.
// 
// \param file [out] if non-NULL, will be set to the file to which the given
// source location points.
// 
// \param line [out] if non-NULL, will be set to the line to which the given
// source location points.
// 
// \param column [out] if non-NULL, will be set to the column to which the given
// source location points.
// 
// \param offset [out] if non-NULL, will be set to the offset into the
// buffer to which the given source location points.
static Napi::Value clang_getFileLocation_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 5) {
        Napi::TypeError::New(env, "Expected 5 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: location (CXSourceLocation)
    CXSourceLocation location = *static_cast<CXSourceLocation*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: file (CXFile *)
    CXFile * file = *static_cast<CXFile **>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: line (unsigned int *)
    unsigned int * line = *static_cast<unsigned int **>(unwrapPointer(info[2].As<Napi::Object>()));
    // Parameter: column (unsigned int *)
    unsigned int * column = *static_cast<unsigned int **>(unwrapPointer(info[3].As<Napi::Object>()));
    // Parameter: offset (unsigned int *)
    unsigned int * offset = *static_cast<unsigned int **>(unwrapPointer(info[4].As<Napi::Object>()));
    
    clang_getFileLocation(location, file, line, column, offset);
    return env.Undefined();
}

// Retrieve the complete file and path name of the given file.
static Napi::Value clang_getFileName_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: SFile (CXFile)
    CXFile SFile = *static_cast<CXFile*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_getFileName(SFile);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Retrieve the last modification time of the given file.
static Napi::Value clang_getFileTime_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: SFile (CXFile)
    CXFile SFile = *static_cast<CXFile*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getFileTime(SFile);
    return Napi::Number::New(env, result);
}

// Retrieve the unique ID for the given \c file.
// 
// \param file the file to get the ID for.
// \param outID stores the returned CXFileUniqueID.
// \returns If there was a failure getting the unique ID, returns non-zero,
// otherwise returns 0.
static Napi::Value clang_getFileUniqueID_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: file (CXFile)
    CXFile file = *static_cast<CXFile*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: outID (CXFileUniqueID *)
    CXFileUniqueID * outID = nullptr;
    if (!info[1].IsNull() && !info[1].IsUndefined() && info[1].IsObject()) {
        outID = static_cast<CXFileUniqueID *>(unwrapPointer(info[1].As<Napi::Object>()));
    }
    
    auto result = clang_getFileUniqueID(file, outID);
    return Napi::Number::New(env, result);
}

// Retrieve the calling convention associated with a function type.
// 
// If a non-function type is passed in, CXCallingConv_Invalid is returned.
static Napi::Value clang_getFunctionTypeCallingConv_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getFunctionTypeCallingConv(T);
    return Napi::Number::New(env, static_cast<int>(result));
}

// For cursors representing an iboutletcollection attribute,
// this function returns the collection element type.
static Napi::Value clang_getIBOutletCollectionType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXType result = clang_getIBOutletCollectionType(arg0);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Retrieve the file that is included by the given inclusion directive
// cursor.
static Napi::Value clang_getIncludedFile_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXFile result = clang_getIncludedFile(cursor);
    CXFile* resultPtr = new CXFile;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXFile");
}

// Visit the set of preprocessor inclusions in a translation unit.
// The visitor function is called with the provided data for every included
// file.  This does not include headers included by the PCH file (unless one
// is inspecting the inclusions in the PCH file itself).
static Napi::Value clang_getInclusions_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: tu (CXTranslationUnit)
    CXTranslationUnit tu = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: visitor (CXInclusionVisitor)
    // Parameter: visitor (CXInclusionVisitor)
    CXInclusionVisitor visitor = nullptr;
    if (info[1].IsExternal()) {
        visitor = reinterpret_cast<CXInclusionVisitor>(info[1].As<Napi::External<void>>().Data());
    }
    // Parameter: client_data (CXClientData)
    CXClientData client_data = *static_cast<CXClientData*>(unwrapPointer(info[2].As<Napi::Object>()));
    
    clang_getInclusions(tu, visitor, client_data);
    return env.Undefined();
}

// Legacy API to retrieve the file, line, column, and offset represented
// by the given source location.
// 
// This interface has been replaced by the newer interface
// #clang_getExpansionLocation(). See that interface's documentation for
// details.
static Napi::Value clang_getInstantiationLocation_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 5) {
        Napi::TypeError::New(env, "Expected 5 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: location (CXSourceLocation)
    CXSourceLocation location = *static_cast<CXSourceLocation*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: file (CXFile *)
    CXFile * file = *static_cast<CXFile **>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: line (unsigned int *)
    unsigned int * line = *static_cast<unsigned int **>(unwrapPointer(info[2].As<Napi::Object>()));
    // Parameter: column (unsigned int *)
    unsigned int * column = *static_cast<unsigned int **>(unwrapPointer(info[3].As<Napi::Object>()));
    // Parameter: offset (unsigned int *)
    unsigned int * offset = *static_cast<unsigned int **>(unwrapPointer(info[4].As<Napi::Object>()));
    
    clang_getInstantiationLocation(location, file, line, column, offset);
    return env.Undefined();
}

// Retrieves the source location associated with a given file/line/column
// in a particular translation unit.
static Napi::Value clang_getLocation_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 4) {
        Napi::TypeError::New(env, "Expected 4 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: tu (CXTranslationUnit)
    CXTranslationUnit tu = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: file (CXFile)
    CXFile file = *static_cast<CXFile*>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: line (unsigned int)
    auto line = info[2].As<Napi::Number>().Uint32Value();
    // Parameter: column (unsigned int)
    auto column = info[3].As<Napi::Number>().Uint32Value();
    
    CXSourceLocation result = clang_getLocation(tu, file, line, column);
    CXSourceLocation* resultPtr = new CXSourceLocation(result);
    return wrapOwnedPointer(env, resultPtr, "CXSourceLocation");
}

// Retrieves the source location associated with a given character offset
// in a particular translation unit.
static Napi::Value clang_getLocationForOffset_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: tu (CXTranslationUnit)
    CXTranslationUnit tu = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: file (CXFile)
    CXFile file = *static_cast<CXFile*>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: offset (unsigned int)
    auto offset = info[2].As<Napi::Number>().Uint32Value();
    
    CXSourceLocation result = clang_getLocationForOffset(tu, file, offset);
    CXSourceLocation* resultPtr = new CXSourceLocation(result);
    return wrapOwnedPointer(env, resultPtr, "CXSourceLocation");
}

// Given a CXFile header file, return the module that contains it, if one
// exists.
static Napi::Value clang_getModuleForFile_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXTranslationUnit)
    CXTranslationUnit arg0 = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: arg1 (CXFile)
    CXFile arg1 = *static_cast<CXFile*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    CXModule result = clang_getModuleForFile(arg0, arg1);
    CXModule* resultPtr = new CXModule;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXModule");
}

// For reference types (e.g., "const int&"), returns the type that the
// reference refers to (e.g "const int").
// 
// Otherwise, returns the type itself.
// 
// A type that has kind \c CXType_LValueReference or
// \c CXType_RValueReference is a reference type.
static Napi::Value clang_getNonReferenceType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: CT (CXType)
    CXType CT = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXType result = clang_getNonReferenceType(CT);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Retrieve the NULL cursor, which represents no entity.
static Napi::Value clang_getNullCursor_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    
    CXCursor result = clang_getNullCursor();
    CXCursor* resultPtr = new CXCursor(result);
    return wrapOwnedPointer(env, resultPtr, "CXCursor");
}

// Retrieve a NULL (invalid) source location.
static Napi::Value clang_getNullLocation_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    
    CXSourceLocation result = clang_getNullLocation();
    CXSourceLocation* resultPtr = new CXSourceLocation(result);
    return wrapOwnedPointer(env, resultPtr, "CXSourceLocation");
}

// Retrieve a NULL (invalid) source range.
static Napi::Value clang_getNullRange_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    
    CXSourceRange result = clang_getNullRange();
    CXSourceRange* resultPtr = new CXSourceRange(result);
    return wrapOwnedPointer(env, resultPtr, "CXSourceRange");
}

// Retrieve the number of non-variadic parameters associated with a
// function type.
// 
// If a non-function type is passed in, -1 is returned.
static Napi::Value clang_getNumArgTypes_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getNumArgTypes(T);
    return Napi::Number::New(env, result);
}

// Retrieve the number of chunks in the given code-completion string.
static Napi::Value clang_getNumCompletionChunks_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: completion_string (CXCompletionString)
    CXCompletionString completion_string = *static_cast<CXCompletionString*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getNumCompletionChunks(completion_string);
    return Napi::Number::New(env, result);
}

// Determine the number of diagnostics produced for the given
// translation unit.
static Napi::Value clang_getNumDiagnostics_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Unit (CXTranslationUnit)
    CXTranslationUnit Unit = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getNumDiagnostics(Unit);
    return Napi::Number::New(env, result);
}

// Determine the number of diagnostics in a CXDiagnosticSet.
static Napi::Value clang_getNumDiagnosticsInSet_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Diags (CXDiagnosticSet)
    CXDiagnosticSet Diags = *static_cast<CXDiagnosticSet*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getNumDiagnosticsInSet(Diags);
    return Napi::Number::New(env, result);
}

// Return the number of elements of an array or vector type.
// 
// If a type is passed in that is not an array or vector type,
// -1 is returned.
static Napi::Value clang_getNumElements_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getNumElements(T);
    return Napi::BigInt::New(env, result);
}

// Determine the number of overloaded declarations referenced by a
// \c CXCursor_OverloadedDeclRef cursor.
// 
// \param cursor The cursor whose overloaded declarations are being queried.
// 
// \returns The number of overloaded declarations referenced by \c cursor. If it
// is not a \c CXCursor_OverloadedDeclRef cursor, returns 0.
static Napi::Value clang_getNumOverloadedDecls_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getNumOverloadedDecls(cursor);
    return Napi::Number::New(env, result);
}

// Returns the offset in bits of a CX_CXXBaseSpecifier relative to the parent
// class.
// 
// Returns a small negative number if the offset cannot be computed. See
// CXTypeLayoutError for error codes.
static Napi::Value clang_getOffsetOfBase_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Parent (CXCursor)
    CXCursor Parent = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: Base (CXCursor)
    CXCursor Base = *static_cast<CXCursor*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    auto result = clang_getOffsetOfBase(Parent, Base);
    return Napi::BigInt::New(env, result);
}

// Retrieve a cursor for one of the overloaded declarations referenced
// by a \c CXCursor_OverloadedDeclRef cursor.
// 
// \param cursor The cursor whose overloaded declarations are being queried.
// 
// \param index The zero-based index into the set of overloaded declarations in
// the cursor.
// 
// \returns A cursor representing the declaration referenced by the given
// \c cursor at the specified \c index. If the cursor does not have an
// associated set of overloaded declarations, or if the index is out of bounds,
// returns \c clang_getNullCursor();
static Napi::Value clang_getOverloadedDecl_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: index (unsigned int)
    auto index = info[1].As<Napi::Number>().Uint32Value();
    
    CXCursor result = clang_getOverloadedDecl(cursor, index);
    CXCursor* resultPtr = new CXCursor(result);
    return wrapOwnedPointer(env, resultPtr, "CXCursor");
}

// Determine the set of methods that are overridden by the given
// method.
// 
// In both Objective-C and C++, a method (aka virtual member function,
// in C++) can override a virtual method in a base class. For
// Objective-C, a method is said to override any method in the class's
// base class, its protocols, or its categories' protocols, that has the same
// selector and is of the same kind (class or instance).
// If no such method exists, the search continues to the class's superclass,
// its protocols, and its categories, and so on. A method from an Objective-C
// implementation is considered to override the same methods as its
// corresponding method in the interface.
// 
// For C++, a virtual member function overrides any virtual member
// function with the same signature that occurs in its base
// classes. With multiple inheritance, a virtual member function can
// override several virtual member functions coming from different
// base classes.
// 
// In all cases, this function determines the immediate overridden
// method, rather than all of the overridden methods. For example, if
// a method is originally declared in a class A, then overridden in B
// (which in inherits from A) and also in C (which inherited from B),
// then the only overridden method returned from this function when
// invoked on C's method will be B's method. The client may then
// invoke this function again, given the previously-found overridden
// methods, to map out the complete method-override set.
// 
// \param cursor A cursor representing an Objective-C or C++
// method. This routine will compute the set of methods that this
// method overrides.
// 
// \param overridden A pointer whose pointee will be replaced with a
// pointer to an array of cursors, representing the set of overridden
// methods. If there are no overridden methods, the pointee will be
// set to NULL. The pointee must be freed via a call to
// \c clang_disposeOverriddenCursors().
// 
// \param num_overridden A pointer to the number of overridden
// functions, will be set to the number of overridden functions in the
// array pointed to by \p overridden.
static Napi::Value clang_getOverriddenCursors_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: cursor (CXCursor)
    CXCursor cursor = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: overridden (CXCursor **)
    CXCursor ** overridden = *static_cast<CXCursor ***>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: num_overridden (unsigned int *)
    unsigned int * num_overridden = *static_cast<unsigned int **>(unwrapPointer(info[2].As<Napi::Object>()));
    
    clang_getOverriddenCursors(cursor, overridden, num_overridden);
    return env.Undefined();
}

// For pointer types, returns the type of the pointee.
static Napi::Value clang_getPointeeType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXType result = clang_getPointeeType(T);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Retrieve the file, line and column represented by the given source
// location, as specified in a # line directive.
// 
// Example: given the following source code in a file somefile.c
// 
// \code
// #123 "dummy.c" 1
// 
// static int func(void)
// {
// return 0;
// }
// \endcode
// 
// the location information returned by this function would be
// 
// File: dummy.c Line: 124 Column: 12
// 
// whereas clang_getExpansionLocation would have returned
// 
// File: somefile.c Line: 3 Column: 12
// 
// \param location the location within a source file that will be decomposed
// into its parts.
// 
// \param filename [out] if non-NULL, will be set to the filename of the
// source location. Note that filenames returned will be for "virtual" files,
// which don't necessarily exist on the machine running clang - e.g. when
// parsing preprocessed output obtained from a different environment. If
// a non-NULL value is passed in, remember to dispose of the returned value
// using \c clang_disposeString() once you've finished with it. For an invalid
// source location, an empty string is returned.
// 
// \param line [out] if non-NULL, will be set to the line number of the
// source location. For an invalid source location, zero is returned.
// 
// \param column [out] if non-NULL, will be set to the column number of the
// source location. For an invalid source location, zero is returned.
static Napi::Value clang_getPresumedLocation_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 4) {
        Napi::TypeError::New(env, "Expected 4 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: location (CXSourceLocation)
    CXSourceLocation location = *static_cast<CXSourceLocation*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: filename (CXString *)
    CXString * filename = nullptr;
    if (!info[1].IsNull() && !info[1].IsUndefined() && info[1].IsObject()) {
        filename = static_cast<CXString *>(unwrapPointer(info[1].As<Napi::Object>()));
    }
    // Parameter: line (unsigned int *)
    unsigned int * line = *static_cast<unsigned int **>(unwrapPointer(info[2].As<Napi::Object>()));
    // Parameter: column (unsigned int *)
    unsigned int * column = *static_cast<unsigned int **>(unwrapPointer(info[3].As<Napi::Object>()));
    
    clang_getPresumedLocation(location, filename, line, column);
    return env.Undefined();
}

// Retrieve a source range given the beginning and ending source
// locations.
static Napi::Value clang_getRange_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: begin (CXSourceLocation)
    CXSourceLocation begin = *static_cast<CXSourceLocation*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: end (CXSourceLocation)
    CXSourceLocation end = *static_cast<CXSourceLocation*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    CXSourceRange result = clang_getRange(begin, end);
    CXSourceRange* resultPtr = new CXSourceRange(result);
    return wrapOwnedPointer(env, resultPtr, "CXSourceRange");
}

// Retrieve a source location representing the last character within a
// source range.
static Napi::Value clang_getRangeEnd_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: range (CXSourceRange)
    CXSourceRange range = *static_cast<CXSourceRange*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXSourceLocation result = clang_getRangeEnd(range);
    CXSourceLocation* resultPtr = new CXSourceLocation(result);
    return wrapOwnedPointer(env, resultPtr, "CXSourceLocation");
}

// Retrieve a source location representing the first character within a
// source range.
static Napi::Value clang_getRangeStart_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: range (CXSourceRange)
    CXSourceRange range = *static_cast<CXSourceRange*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXSourceLocation result = clang_getRangeStart(range);
    CXSourceLocation* resultPtr = new CXSourceLocation(result);
    return wrapOwnedPointer(env, resultPtr, "CXSourceLocation");
}

// Retrieve a remapping.
// 
// \param path the path that contains metadata about remappings.
// 
// \returns the requested remapping. This remapping must be freed
// via a call to \c clang_remap_dispose(). Can return NULL if an error occurred.
static Napi::Value clang_getRemappings_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: path (const char *)
    std::string path_str = info[0].As<Napi::String>().Utf8Value();
    const char* path = path_str.c_str();
    
    CXRemapping result = clang_getRemappings(path);
    CXRemapping* resultPtr = new CXRemapping;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXRemapping");
}

// Retrieve a remapping.
// 
// \param filePaths pointer to an array of file paths containing remapping info.
// 
// \param numFiles number of file paths.
// 
// \returns the requested remapping. This remapping must be freed
// via a call to \c clang_remap_dispose(). Can return NULL if an error occurred.
static Napi::Value clang_getRemappingsFromFileList_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: filePaths (const char **)
    const char ** filePaths = *static_cast<const char ***>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: numFiles (unsigned int)
    auto numFiles = info[1].As<Napi::Number>().Uint32Value();
    
    CXRemapping result = clang_getRemappingsFromFileList(filePaths, numFiles);
    CXRemapping* resultPtr = new CXRemapping;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXRemapping");
}

// Retrieve the return type associated with a function type.
// 
// If a non-function type is passed in, an invalid type is returned.
static Napi::Value clang_getResultType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXType result = clang_getResultType(T);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Retrieve all ranges that were skipped by the preprocessor.
// 
// The preprocessor will skip lines when they are surrounded by an
// if/ifdef/ifndef directive whose condition does not evaluate to true.
static Napi::Value clang_getSkippedRanges_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: tu (CXTranslationUnit)
    CXTranslationUnit tu = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: file (CXFile)
    CXFile file = *static_cast<CXFile*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    CXSourceRangeList * result = clang_getSkippedRanges(tu, file);
    CXSourceRangeList ** resultPtr = new CXSourceRangeList *;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXSourceRangeList *");
}

// Given a cursor that may represent a specialization or instantiation
// of a template, retrieve the cursor that represents the template that it
// specializes or from which it was instantiated.
// 
// This routine determines the template involved both for explicit
// specializations of templates and for implicit instantiations of the template,
// both of which are referred to as "specializations". For a class template
// specialization (e.g., \c std::vector<bool>), this routine will return
// either the primary template (\c std::vector) or, if the specialization was
// instantiated from a class template partial specialization, the class template
// partial specialization. For a class template partial specialization and a
// function template specialization (including instantiations), this
// this routine will return the specialized template.
// 
// For members of a class template (e.g., member functions, member classes, or
// static data members), returns the specialized or instantiated member.
// Although not strictly "templates" in the C++ language, members of class
// templates have the same notions of specializations and instantiations that
// templates do, so this routine treats them similarly.
// 
// \param C A cursor that may be a specialization of a template or a member
// of a template.
// 
// \returns If the given cursor is a specialization or instantiation of a
// template or a member thereof, the template or member that it specializes or
// from which it was instantiated. Otherwise, returns a NULL cursor.
static Napi::Value clang_getSpecializedCursorTemplate_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXCursor result = clang_getSpecializedCursorTemplate(C);
    CXCursor* resultPtr = new CXCursor(result);
    return wrapOwnedPointer(env, resultPtr, "CXCursor");
}

// Retrieve the file, line, column, and offset represented by
// the given source location.
// 
// If the location refers into a macro instantiation, return where the
// location was originally spelled in the source file.
// 
// \param location the location within a source file that will be decomposed
// into its parts.
// 
// \param file [out] if non-NULL, will be set to the file to which the given
// source location points.
// 
// \param line [out] if non-NULL, will be set to the line to which the given
// source location points.
// 
// \param column [out] if non-NULL, will be set to the column to which the given
// source location points.
// 
// \param offset [out] if non-NULL, will be set to the offset into the
// buffer to which the given source location points.
static Napi::Value clang_getSpellingLocation_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 5) {
        Napi::TypeError::New(env, "Expected 5 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: location (CXSourceLocation)
    CXSourceLocation location = *static_cast<CXSourceLocation*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: file (CXFile *)
    CXFile * file = *static_cast<CXFile **>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: line (unsigned int *)
    unsigned int * line = *static_cast<unsigned int **>(unwrapPointer(info[2].As<Napi::Object>()));
    // Parameter: column (unsigned int *)
    unsigned int * column = *static_cast<unsigned int **>(unwrapPointer(info[3].As<Napi::Object>()));
    // Parameter: offset (unsigned int *)
    unsigned int * offset = *static_cast<unsigned int **>(unwrapPointer(info[4].As<Napi::Object>()));
    
    clang_getSpellingLocation(location, file, line, column, offset);
    return env.Undefined();
}

// Given a cursor that represents a template, determine
// the cursor kind of the specializations would be generated by instantiating
// the template.
// 
// This routine can be used to determine what flavor of function template,
// class template, or class template partial specialization is stored in the
// cursor. For example, it can describe whether a class template cursor is
// declared with "struct", "class" or "union".
// 
// \param C The cursor to query. This cursor should represent a template
// declaration.
// 
// \returns The cursor kind of the specializations that would be generated
// by instantiating the template \p C. If \p C is not a template, returns
// \c CXCursor_NoDeclFound.
static Napi::Value clang_getTemplateCursorKind_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getTemplateCursorKind(C);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Get the raw lexical token starting with the given location.
// 
// \param TU the translation unit whose text is being tokenized.
// 
// \param Location the source location with which the token starts.
// 
// \returns The token starting with the given location or NULL if no such token
// exist. The returned pointer must be freed with clang_disposeTokens before the
// translation unit is destroyed.
static Napi::Value clang_getToken_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: TU (CXTranslationUnit)
    CXTranslationUnit TU = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: Location (CXSourceLocation)
    CXSourceLocation Location = *static_cast<CXSourceLocation*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    CXToken * result = clang_getToken(TU, Location);
    CXToken ** resultPtr = new CXToken *;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXToken *");
}

// Retrieve a source range that covers the given token.
static Napi::Value clang_getTokenExtent_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXTranslationUnit)
    CXTranslationUnit arg0 = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: arg1 (CXToken)
    CXToken arg1 = *static_cast<CXToken*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    CXSourceRange result = clang_getTokenExtent(arg0, arg1);
    CXSourceRange* resultPtr = new CXSourceRange(result);
    return wrapOwnedPointer(env, resultPtr, "CXSourceRange");
}

// Determine the kind of the given token.
static Napi::Value clang_getTokenKind_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXToken)
    CXToken arg0 = *static_cast<CXToken*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_getTokenKind(arg0);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Retrieve the source location of the given token.
static Napi::Value clang_getTokenLocation_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXTranslationUnit)
    CXTranslationUnit arg0 = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: arg1 (CXToken)
    CXToken arg1 = *static_cast<CXToken*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    CXSourceLocation result = clang_getTokenLocation(arg0, arg1);
    CXSourceLocation* resultPtr = new CXSourceLocation(result);
    return wrapOwnedPointer(env, resultPtr, "CXSourceLocation");
}

// Determine the spelling of the given token.
// 
// The spelling of a token is the textual representation of that token, e.g.,
// the text of an identifier or keyword.
static Napi::Value clang_getTokenSpelling_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXTranslationUnit)
    CXTranslationUnit arg0 = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: arg1 (CXToken)
    CXToken arg1 = *static_cast<CXToken*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    CXString result = clang_getTokenSpelling(arg0, arg1);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Retrieve the cursor that represents the given translation unit.
// 
// The translation unit cursor can be used to start traversing the
// various declarations within the given translation unit.
static Napi::Value clang_getTranslationUnitCursor_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXTranslationUnit)
    CXTranslationUnit arg0 = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXCursor result = clang_getTranslationUnitCursor(arg0);
    CXCursor* resultPtr = new CXCursor(result);
    return wrapOwnedPointer(env, resultPtr, "CXCursor");
}

// Get the original translation unit source file name.
static Napi::Value clang_getTranslationUnitSpelling_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: CTUnit (CXTranslationUnit)
    CXTranslationUnit CTUnit = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_getTranslationUnitSpelling(CTUnit);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Get target information for this translation unit.
// 
// The CXTargetInfo object cannot outlive the CXTranslationUnit object.
static Napi::Value clang_getTranslationUnitTargetInfo_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: CTUnit (CXTranslationUnit)
    CXTranslationUnit CTUnit = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXTargetInfo result = clang_getTranslationUnitTargetInfo(CTUnit);
    CXTargetInfo* resultPtr = new CXTargetInfo;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXTargetInfo");
}

// Returns the human-readable null-terminated C string that represents
// the name of the memory category.  This string should never be freed.
static Napi::Value clang_getTUResourceUsageName_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: kind (enum CXTUResourceUsageKind)
    enum CXTUResourceUsageKind kind = static_cast<enum CXTUResourceUsageKind>(info[0].As<Napi::Number>().Int32Value());
    
    auto result = clang_getTUResourceUsageName(kind);
    if (result == nullptr) {
        return env.Null();
    }
    return Napi::String::New(env, result);
}

// Return the cursor for the declaration of the given type.
static Napi::Value clang_getTypeDeclaration_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXCursor result = clang_getTypeDeclaration(T);
    CXCursor* resultPtr = new CXCursor(result);
    return wrapOwnedPointer(env, resultPtr, "CXCursor");
}

// Retrieve the underlying type of a typedef declaration.
// 
// If the cursor does not reference a typedef declaration, an invalid type is
// returned.
static Napi::Value clang_getTypedefDeclUnderlyingType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: C (CXCursor)
    CXCursor C = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXType result = clang_getTypedefDeclUnderlyingType(C);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Returns the typedef name of the given type.
static Napi::Value clang_getTypedefName_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: CT (CXType)
    CXType CT = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_getTypedefName(CT);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Retrieve the spelling of a given CXTypeKind.
static Napi::Value clang_getTypeKindSpelling_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: K (enum CXTypeKind)
    enum CXTypeKind K = static_cast<enum CXTypeKind>(info[0].As<Napi::Number>().Int32Value());
    
    CXString result = clang_getTypeKindSpelling(K);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Pretty-print the underlying type using a custom printing policy.
// 
// If the type is invalid, an empty string is returned.
static Napi::Value clang_getTypePrettyPrinted_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: CT (CXType)
    CXType CT = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: cxPolicy (CXPrintingPolicy)
    CXPrintingPolicy cxPolicy = *static_cast<CXPrintingPolicy*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    CXString result = clang_getTypePrettyPrinted(CT, cxPolicy);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Pretty-print the underlying type using the rules of the
// language of the translation unit from which it came.
// 
// If the type is invalid, an empty string is returned.
static Napi::Value clang_getTypeSpelling_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: CT (CXType)
    CXType CT = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_getTypeSpelling(CT);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Retrieve the spelling of a given CXUnaryOperatorKind.
static Napi::Value clang_getUnaryOperatorKindSpelling_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: kind (enum CXUnaryOperatorKind)
    enum CXUnaryOperatorKind kind = static_cast<enum CXUnaryOperatorKind>(info[0].As<Napi::Number>().Int32Value());
    
    CXString result = clang_getUnaryOperatorKindSpelling(kind);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Retrieve the unqualified variant of the given type, removing as
// little sugar as possible.
// 
// For example, given the following series of typedefs:
// 
// \code
// typedef int Integer;
// typedef const Integer CInteger;
// typedef CInteger DifferenceType;
// \endcode
// 
// Executing \c clang_getUnqualifiedType() on a \c CXType that
// represents \c DifferenceType, will desugar to a type representing
// \c Integer, that has no qualifiers.
// 
// And, executing \c clang_getUnqualifiedType() on the type of the
// first argument of the following function declaration:
// 
// \code
// void foo(const int);
// \endcode
// 
// Will return a type representing \c int, removing the \c const
// qualifier.
// 
// Sugar over array types is not desugared.
// 
// A type can be checked for qualifiers with \c
// clang_isConstQualifiedType(), \c clang_isVolatileQualifiedType()
// and \c clang_isRestrictQualifiedType().
// 
// A type that resulted from a call to \c clang_getUnqualifiedType
// will return \c false for all of the above calls.
static Napi::Value clang_getUnqualifiedType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: CT (CXType)
    CXType CT = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXType result = clang_getUnqualifiedType(CT);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Compute a hash value for the given cursor.
static Napi::Value clang_hashCursor_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_hashCursor(arg0);
    return Napi::Number::New(env, result);
}

// For retrieving a custom CXIdxClientContainer attached to a
// container.
static Napi::Value clang_index_getClientContainer_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (const CXIdxContainerInfo *)
    const CXIdxContainerInfo * arg0 = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        arg0 = static_cast<const CXIdxContainerInfo *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    
    CXIdxClientContainer result = clang_index_getClientContainer(arg0);
    CXIdxClientContainer* resultPtr = new CXIdxClientContainer;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXIdxClientContainer");
}

// For retrieving a custom CXIdxClientEntity attached to an entity.
static Napi::Value clang_index_getClientEntity_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (const CXIdxEntityInfo *)
    const CXIdxEntityInfo * arg0 = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        arg0 = static_cast<const CXIdxEntityInfo *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    
    CXIdxClientEntity result = clang_index_getClientEntity(arg0);
    CXIdxClientEntity* resultPtr = new CXIdxClientEntity;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXIdxClientEntity");
}

static Napi::Value clang_index_getCXXClassDeclInfo_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (const CXIdxDeclInfo *)
    const CXIdxDeclInfo * arg0 = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        arg0 = static_cast<const CXIdxDeclInfo *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    
    const CXIdxCXXClassDeclInfo * result = clang_index_getCXXClassDeclInfo(arg0);
    const CXIdxCXXClassDeclInfo ** resultPtr = new const CXIdxCXXClassDeclInfo *;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "const CXIdxCXXClassDeclInfo *");
}

static Napi::Value clang_index_getIBOutletCollectionAttrInfo_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (const CXIdxAttrInfo *)
    const CXIdxAttrInfo * arg0 = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        arg0 = static_cast<const CXIdxAttrInfo *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    
    const CXIdxIBOutletCollectionAttrInfo * result = clang_index_getIBOutletCollectionAttrInfo(arg0);
    const CXIdxIBOutletCollectionAttrInfo ** resultPtr = new const CXIdxIBOutletCollectionAttrInfo *;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "const CXIdxIBOutletCollectionAttrInfo *");
}

static Napi::Value clang_index_getObjCCategoryDeclInfo_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (const CXIdxDeclInfo *)
    const CXIdxDeclInfo * arg0 = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        arg0 = static_cast<const CXIdxDeclInfo *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    
    const CXIdxObjCCategoryDeclInfo * result = clang_index_getObjCCategoryDeclInfo(arg0);
    const CXIdxObjCCategoryDeclInfo ** resultPtr = new const CXIdxObjCCategoryDeclInfo *;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "const CXIdxObjCCategoryDeclInfo *");
}

static Napi::Value clang_index_getObjCContainerDeclInfo_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (const CXIdxDeclInfo *)
    const CXIdxDeclInfo * arg0 = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        arg0 = static_cast<const CXIdxDeclInfo *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    
    const CXIdxObjCContainerDeclInfo * result = clang_index_getObjCContainerDeclInfo(arg0);
    const CXIdxObjCContainerDeclInfo ** resultPtr = new const CXIdxObjCContainerDeclInfo *;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "const CXIdxObjCContainerDeclInfo *");
}

static Napi::Value clang_index_getObjCInterfaceDeclInfo_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (const CXIdxDeclInfo *)
    const CXIdxDeclInfo * arg0 = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        arg0 = static_cast<const CXIdxDeclInfo *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    
    const CXIdxObjCInterfaceDeclInfo * result = clang_index_getObjCInterfaceDeclInfo(arg0);
    const CXIdxObjCInterfaceDeclInfo ** resultPtr = new const CXIdxObjCInterfaceDeclInfo *;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "const CXIdxObjCInterfaceDeclInfo *");
}

static Napi::Value clang_index_getObjCPropertyDeclInfo_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (const CXIdxDeclInfo *)
    const CXIdxDeclInfo * arg0 = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        arg0 = static_cast<const CXIdxDeclInfo *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    
    const CXIdxObjCPropertyDeclInfo * result = clang_index_getObjCPropertyDeclInfo(arg0);
    const CXIdxObjCPropertyDeclInfo ** resultPtr = new const CXIdxObjCPropertyDeclInfo *;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "const CXIdxObjCPropertyDeclInfo *");
}

static Napi::Value clang_index_getObjCProtocolRefListInfo_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (const CXIdxDeclInfo *)
    const CXIdxDeclInfo * arg0 = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        arg0 = static_cast<const CXIdxDeclInfo *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    
    const CXIdxObjCProtocolRefListInfo * result = clang_index_getObjCProtocolRefListInfo(arg0);
    const CXIdxObjCProtocolRefListInfo ** resultPtr = new const CXIdxObjCProtocolRefListInfo *;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "const CXIdxObjCProtocolRefListInfo *");
}

static Napi::Value clang_index_isEntityObjCContainerKind_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXIdxEntityKind)
    CXIdxEntityKind arg0 = static_cast<CXIdxEntityKind>(info[0].As<Napi::Number>().Int32Value());
    
    auto result = clang_index_isEntityObjCContainerKind(arg0);
    return Napi::Number::New(env, result);
}

// For setting a custom CXIdxClientContainer attached to a
// container.
static Napi::Value clang_index_setClientContainer_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (const CXIdxContainerInfo *)
    const CXIdxContainerInfo * arg0 = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        arg0 = static_cast<const CXIdxContainerInfo *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    // Parameter: arg1 (CXIdxClientContainer)
    CXIdxClientContainer arg1 = *static_cast<CXIdxClientContainer*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    clang_index_setClientContainer(arg0, arg1);
    return env.Undefined();
}

// For setting a custom CXIdxClientEntity attached to an entity.
static Napi::Value clang_index_setClientEntity_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (const CXIdxEntityInfo *)
    const CXIdxEntityInfo * arg0 = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        arg0 = static_cast<const CXIdxEntityInfo *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    // Parameter: arg1 (CXIdxClientEntity)
    CXIdxClientEntity arg1 = *static_cast<CXIdxClientEntity*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    clang_index_setClientEntity(arg0, arg1);
    return env.Undefined();
}

// An indexing action/session, to be applied to one or multiple
// translation units.
// 
// \param CIdx The index object with which the index action will be associated.
static Napi::Value clang_IndexAction_create_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: CIdx (CXIndex)
    CXIndex CIdx = *static_cast<CXIndex*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXIndexAction result = clang_IndexAction_create(CIdx);
    CXIndexAction* resultPtr = new CXIndexAction;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXIndexAction");
}

// Destroy the given index action.
// 
// The index action must not be destroyed until all of the translation units
// created within that index action have been destroyed.
static Napi::Value clang_IndexAction_dispose_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXIndexAction)
    CXIndexAction arg0 = *static_cast<CXIndexAction*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    clang_IndexAction_dispose(arg0);
    return env.Undefined();
}

// Retrieve the CXSourceLocation represented by the given CXIdxLoc.
static Napi::Value clang_indexLoc_getCXSourceLocation_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: loc (CXIdxLoc)
    CXIdxLoc loc = *static_cast<CXIdxLoc*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXSourceLocation result = clang_indexLoc_getCXSourceLocation(loc);
    CXSourceLocation* resultPtr = new CXSourceLocation(result);
    return wrapOwnedPointer(env, resultPtr, "CXSourceLocation");
}

// Retrieve the CXIdxFile, file, line, column, and offset represented by
// the given CXIdxLoc.
// 
// If the location refers into a macro expansion, retrieves the
// location of the macro expansion and if it refers into a macro argument
// retrieves the location of the argument.
static Napi::Value clang_indexLoc_getFileLocation_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 6) {
        Napi::TypeError::New(env, "Expected 6 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: loc (CXIdxLoc)
    CXIdxLoc loc = *static_cast<CXIdxLoc*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: indexFile (CXIdxClientFile *)
    CXIdxClientFile * indexFile = *static_cast<CXIdxClientFile **>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: file (CXFile *)
    CXFile * file = *static_cast<CXFile **>(unwrapPointer(info[2].As<Napi::Object>()));
    // Parameter: line (unsigned int *)
    unsigned int * line = *static_cast<unsigned int **>(unwrapPointer(info[3].As<Napi::Object>()));
    // Parameter: column (unsigned int *)
    unsigned int * column = *static_cast<unsigned int **>(unwrapPointer(info[4].As<Napi::Object>()));
    // Parameter: offset (unsigned int *)
    unsigned int * offset = *static_cast<unsigned int **>(unwrapPointer(info[5].As<Napi::Object>()));
    
    clang_indexLoc_getFileLocation(loc, indexFile, file, line, column, offset);
    return env.Undefined();
}

// Index the given source file and the translation unit corresponding
// to that file via callbacks implemented through #IndexerCallbacks.
// 
// \param client_data pointer data supplied by the client, which will
// be passed to the invoked callbacks.
// 
// \param index_callbacks Pointer to indexing callbacks that the client
// implements.
// 
// \param index_callbacks_size Size of #IndexerCallbacks structure that gets
// passed in index_callbacks.
// 
// \param index_options A bitmask of options that affects how indexing is
// performed. This should be a bitwise OR of the CXIndexOpt_XXX flags.
// 
// \param[out] out_TU pointer to store a \c CXTranslationUnit that can be
// reused after indexing is finished. Set to \c NULL if you do not require it.
// 
// \returns 0 on success or if there were errors from which the compiler could
// recover.  If there is a failure from which there is no recovery, returns
// a non-zero \c CXErrorCode.
// 
// The rest of the parameters are the same as #clang_parseTranslationUnit.
static Napi::Value clang_indexSourceFile_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 12) {
        Napi::TypeError::New(env, "Expected 12 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXIndexAction)
    CXIndexAction arg0 = *static_cast<CXIndexAction*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: client_data (CXClientData)
    CXClientData client_data = *static_cast<CXClientData*>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: index_callbacks (IndexerCallbacks *)
    IndexerCallbacks * index_callbacks = nullptr;
    if (!info[2].IsNull() && !info[2].IsUndefined() && info[2].IsObject()) {
        index_callbacks = static_cast<IndexerCallbacks *>(unwrapPointer(info[2].As<Napi::Object>()));
    }
    // Parameter: index_callbacks_size (unsigned int)
    auto index_callbacks_size = info[3].As<Napi::Number>().Uint32Value();
    // Parameter: index_options (unsigned int)
    auto index_options = info[4].As<Napi::Number>().Uint32Value();
    // Parameter: source_filename (const char *)
    std::string source_filename_str = info[5].As<Napi::String>().Utf8Value();
    const char* source_filename = source_filename_str.c_str();
    // Parameter: command_line_args (const char *const *)
    const char *const * command_line_args = nullptr;
    std::vector<std::string> command_line_args_strings;
    std::vector<const char*> command_line_args_cstrs;
    if (!info[6].IsNull() && !info[6].IsUndefined()) {
        if (info[6].IsArray()) {
            Napi::Array arr = info[6].As<Napi::Array>();
            for (uint32_t j = 0; j < arr.Length(); j++) {
                command_line_args_strings.push_back(arr.Get(j).As<Napi::String>().Utf8Value());
            }
            for (const auto& s : command_line_args_strings) {
                command_line_args_cstrs.push_back(s.c_str());
            }
            command_line_args = command_line_args_cstrs.data();
        } else if (info[6].IsObject()) {
            // Assume it's a wrapped pointer
            command_line_args = *static_cast<const char *const **>(unwrapPointer(info[6].As<Napi::Object>()));
        }
    }
    // Parameter: num_command_line_args (int)
    auto num_command_line_args = info[7].As<Napi::Number>().Int32Value();
    // Parameter: unsaved_files (struct CXUnsavedFile *)
    struct CXUnsavedFile * unsaved_files = nullptr;
    if (!info[8].IsNull() && !info[8].IsUndefined() && info[8].IsObject()) {
        unsaved_files = static_cast<struct CXUnsavedFile *>(unwrapPointer(info[8].As<Napi::Object>()));
    }
    // Parameter: num_unsaved_files (unsigned int)
    auto num_unsaved_files = info[9].As<Napi::Number>().Uint32Value();
    // Parameter: out_TU (CXTranslationUnit *)
    CXTranslationUnit * out_TU = *static_cast<CXTranslationUnit **>(unwrapPointer(info[10].As<Napi::Object>()));
    // Parameter: TU_options (unsigned int)
    auto TU_options = info[11].As<Napi::Number>().Uint32Value();
    
    auto result = clang_indexSourceFile(arg0, client_data, index_callbacks, index_callbacks_size, index_options, source_filename, command_line_args, num_command_line_args, unsaved_files, num_unsaved_files, out_TU, TU_options);
    return Napi::Number::New(env, result);
}

// Same as clang_indexSourceFile but requires a full command line
// for \c command_line_args including argv[0]. This is useful if the standard
// library paths are relative to the binary.
static Napi::Value clang_indexSourceFileFullArgv_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 12) {
        Napi::TypeError::New(env, "Expected 12 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXIndexAction)
    CXIndexAction arg0 = *static_cast<CXIndexAction*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: client_data (CXClientData)
    CXClientData client_data = *static_cast<CXClientData*>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: index_callbacks (IndexerCallbacks *)
    IndexerCallbacks * index_callbacks = nullptr;
    if (!info[2].IsNull() && !info[2].IsUndefined() && info[2].IsObject()) {
        index_callbacks = static_cast<IndexerCallbacks *>(unwrapPointer(info[2].As<Napi::Object>()));
    }
    // Parameter: index_callbacks_size (unsigned int)
    auto index_callbacks_size = info[3].As<Napi::Number>().Uint32Value();
    // Parameter: index_options (unsigned int)
    auto index_options = info[4].As<Napi::Number>().Uint32Value();
    // Parameter: source_filename (const char *)
    std::string source_filename_str = info[5].As<Napi::String>().Utf8Value();
    const char* source_filename = source_filename_str.c_str();
    // Parameter: command_line_args (const char *const *)
    const char *const * command_line_args = nullptr;
    std::vector<std::string> command_line_args_strings;
    std::vector<const char*> command_line_args_cstrs;
    if (!info[6].IsNull() && !info[6].IsUndefined()) {
        if (info[6].IsArray()) {
            Napi::Array arr = info[6].As<Napi::Array>();
            for (uint32_t j = 0; j < arr.Length(); j++) {
                command_line_args_strings.push_back(arr.Get(j).As<Napi::String>().Utf8Value());
            }
            for (const auto& s : command_line_args_strings) {
                command_line_args_cstrs.push_back(s.c_str());
            }
            command_line_args = command_line_args_cstrs.data();
        } else if (info[6].IsObject()) {
            // Assume it's a wrapped pointer
            command_line_args = *static_cast<const char *const **>(unwrapPointer(info[6].As<Napi::Object>()));
        }
    }
    // Parameter: num_command_line_args (int)
    auto num_command_line_args = info[7].As<Napi::Number>().Int32Value();
    // Parameter: unsaved_files (struct CXUnsavedFile *)
    struct CXUnsavedFile * unsaved_files = nullptr;
    if (!info[8].IsNull() && !info[8].IsUndefined() && info[8].IsObject()) {
        unsaved_files = static_cast<struct CXUnsavedFile *>(unwrapPointer(info[8].As<Napi::Object>()));
    }
    // Parameter: num_unsaved_files (unsigned int)
    auto num_unsaved_files = info[9].As<Napi::Number>().Uint32Value();
    // Parameter: out_TU (CXTranslationUnit *)
    CXTranslationUnit * out_TU = *static_cast<CXTranslationUnit **>(unwrapPointer(info[10].As<Napi::Object>()));
    // Parameter: TU_options (unsigned int)
    auto TU_options = info[11].As<Napi::Number>().Uint32Value();
    
    auto result = clang_indexSourceFileFullArgv(arg0, client_data, index_callbacks, index_callbacks_size, index_options, source_filename, command_line_args, num_command_line_args, unsaved_files, num_unsaved_files, out_TU, TU_options);
    return Napi::Number::New(env, result);
}

// Index the given translation unit via callbacks implemented through
// #IndexerCallbacks.
// 
// The order of callback invocations is not guaranteed to be the same as
// when indexing a source file. The high level order will be:
// 
// -Preprocessor callbacks invocations
// -Declaration/reference callbacks invocations
// -Diagnostic callback invocations
// 
// The parameters are the same as #clang_indexSourceFile.
// 
// \returns If there is a failure from which there is no recovery, returns
// non-zero, otherwise returns 0.
static Napi::Value clang_indexTranslationUnit_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 6) {
        Napi::TypeError::New(env, "Expected 6 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXIndexAction)
    CXIndexAction arg0 = *static_cast<CXIndexAction*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: client_data (CXClientData)
    CXClientData client_data = *static_cast<CXClientData*>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: index_callbacks (IndexerCallbacks *)
    IndexerCallbacks * index_callbacks = nullptr;
    if (!info[2].IsNull() && !info[2].IsUndefined() && info[2].IsObject()) {
        index_callbacks = static_cast<IndexerCallbacks *>(unwrapPointer(info[2].As<Napi::Object>()));
    }
    // Parameter: index_callbacks_size (unsigned int)
    auto index_callbacks_size = info[3].As<Napi::Number>().Uint32Value();
    // Parameter: index_options (unsigned int)
    auto index_options = info[4].As<Napi::Number>().Uint32Value();
    // Parameter: arg5 (CXTranslationUnit)
    CXTranslationUnit arg5 = *static_cast<CXTranslationUnit*>(unwrapPointer(info[5].As<Napi::Object>()));
    
    auto result = clang_indexTranslationUnit(arg0, client_data, index_callbacks, index_callbacks_size, index_options, arg5);
    return Napi::Number::New(env, result);
}

// Determine whether the given cursor kind represents an attribute.
static Napi::Value clang_isAttribute_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (enum CXCursorKind)
    enum CXCursorKind arg0 = static_cast<enum CXCursorKind>(info[0].As<Napi::Number>().Int32Value());
    
    auto result = clang_isAttribute(arg0);
    return Napi::Number::New(env, result);
}

// Determine for two source locations if the first comes
// strictly before the second one in the source code.
// 
// \returns non-zero if the first source location comes
// strictly before the second one, zero otherwise.
static Napi::Value clang_isBeforeInTranslationUnit_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: loc1 (CXSourceLocation)
    CXSourceLocation loc1 = *static_cast<CXSourceLocation*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: loc2 (CXSourceLocation)
    CXSourceLocation loc2 = *static_cast<CXSourceLocation*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    auto result = clang_isBeforeInTranslationUnit(loc1, loc2);
    return Napi::Number::New(env, result);
}

// Determine whether a CXType has the "const" qualifier set,
// without looking through typedefs that may have added "const" at a
// different level.
static Napi::Value clang_isConstQualifiedType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_isConstQualifiedType(T);
    return Napi::Number::New(env, result);
}

// Determine whether the declaration pointed to by this cursor
// is also a definition of that entity.
static Napi::Value clang_isCursorDefinition_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_isCursorDefinition(arg0);
    return Napi::Number::New(env, result);
}

// Determine whether the given cursor kind represents a declaration.
static Napi::Value clang_isDeclaration_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (enum CXCursorKind)
    enum CXCursorKind arg0 = static_cast<enum CXCursorKind>(info[0].As<Napi::Number>().Int32Value());
    
    auto result = clang_isDeclaration(arg0);
    return Napi::Number::New(env, result);
}

// Determine whether the given cursor kind represents an expression.
static Napi::Value clang_isExpression_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (enum CXCursorKind)
    enum CXCursorKind arg0 = static_cast<enum CXCursorKind>(info[0].As<Napi::Number>().Int32Value());
    
    auto result = clang_isExpression(arg0);
    return Napi::Number::New(env, result);
}

// Determine whether the given header is guarded against
// multiple inclusions, either with the conventional
// \#ifndef/\#define/\#endif macro guards or with \#pragma once.
static Napi::Value clang_isFileMultipleIncludeGuarded_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: tu (CXTranslationUnit)
    CXTranslationUnit tu = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: file (CXFile)
    CXFile file = *static_cast<CXFile*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    auto result = clang_isFileMultipleIncludeGuarded(tu, file);
    return Napi::Number::New(env, result);
}

// Return 1 if the CXType is a variadic function type, and 0 otherwise.
static Napi::Value clang_isFunctionTypeVariadic_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_isFunctionTypeVariadic(T);
    return Napi::Number::New(env, result);
}

// Determine whether the given cursor kind represents an invalid
// cursor.
static Napi::Value clang_isInvalid_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (enum CXCursorKind)
    enum CXCursorKind arg0 = static_cast<enum CXCursorKind>(info[0].As<Napi::Number>().Int32Value());
    
    auto result = clang_isInvalid(arg0);
    return Napi::Number::New(env, result);
}

// Determine whether the given declaration is invalid.
// 
// A declaration is invalid if it could not be parsed successfully.
// 
// \returns non-zero if the cursor represents a declaration and it is
// invalid, otherwise NULL.
static Napi::Value clang_isInvalidDeclaration_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_isInvalidDeclaration(arg0);
    return Napi::Number::New(env, result);
}

// Return 1 if the CXType is a POD (plain old data) type, and 0
// otherwise.
static Napi::Value clang_isPODType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_isPODType(T);
    return Napi::Number::New(env, result);
}

// *
// Determine whether the given cursor represents a preprocessing
// element, such as a preprocessor directive or macro instantiation.
static Napi::Value clang_isPreprocessing_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (enum CXCursorKind)
    enum CXCursorKind arg0 = static_cast<enum CXCursorKind>(info[0].As<Napi::Number>().Int32Value());
    
    auto result = clang_isPreprocessing(arg0);
    return Napi::Number::New(env, result);
}

// Determine whether the given cursor kind represents a simple
// reference.
// 
// Note that other kinds of cursors (such as expressions) can also refer to
// other cursors. Use clang_getCursorReferenced() to determine whether a
// particular cursor refers to another entity.
static Napi::Value clang_isReference_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (enum CXCursorKind)
    enum CXCursorKind arg0 = static_cast<enum CXCursorKind>(info[0].As<Napi::Number>().Int32Value());
    
    auto result = clang_isReference(arg0);
    return Napi::Number::New(env, result);
}

// Determine whether a CXType has the "restrict" qualifier set,
// without looking through typedefs that may have added "restrict" at a
// different level.
static Napi::Value clang_isRestrictQualifiedType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_isRestrictQualifiedType(T);
    return Napi::Number::New(env, result);
}

// Determine whether the given cursor kind represents a statement.
static Napi::Value clang_isStatement_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (enum CXCursorKind)
    enum CXCursorKind arg0 = static_cast<enum CXCursorKind>(info[0].As<Napi::Number>().Int32Value());
    
    auto result = clang_isStatement(arg0);
    return Napi::Number::New(env, result);
}

// Determine whether the given cursor kind represents a translation
// unit.
static Napi::Value clang_isTranslationUnit_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (enum CXCursorKind)
    enum CXCursorKind arg0 = static_cast<enum CXCursorKind>(info[0].As<Napi::Number>().Int32Value());
    
    auto result = clang_isTranslationUnit(arg0);
    return Napi::Number::New(env, result);
}

// *
// Determine whether the given cursor represents a currently
// unexposed piece of the AST (e.g., CXCursor_UnexposedStmt).
static Napi::Value clang_isUnexposed_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (enum CXCursorKind)
    enum CXCursorKind arg0 = static_cast<enum CXCursorKind>(info[0].As<Napi::Number>().Int32Value());
    
    auto result = clang_isUnexposed(arg0);
    return Napi::Number::New(env, result);
}

// Returns 1 if the base class specified by the cursor with kind
// CX_CXXBaseSpecifier is virtual.
static Napi::Value clang_isVirtualBase_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXCursor)
    CXCursor arg0 = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_isVirtualBase(arg0);
    return Napi::Number::New(env, result);
}

// Determine whether a CXType has the "volatile" qualifier set,
// without looking through typedefs that may have added "volatile" at
// a different level.
static Napi::Value clang_isVolatileQualifiedType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_isVolatileQualifiedType(T);
    return Napi::Number::New(env, result);
}

// Deserialize a set of diagnostics from a Clang diagnostics bitcode
// file.
// 
// \param file The name of the file to deserialize.
// \param error A pointer to a enum value recording if there was a problem
// deserializing the diagnostics.
// \param errorString A pointer to a CXString for recording the error string
// if the file was not successfully loaded.
// 
// \returns A loaded CXDiagnosticSet if successful, and NULL otherwise.  These
// diagnostics should be released using clang_disposeDiagnosticSet().
static Napi::Value clang_loadDiagnostics_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: file (const char *)
    std::string file_str = info[0].As<Napi::String>().Utf8Value();
    const char* file = file_str.c_str();
    // Parameter: error (enum CXLoadDiag_Error *)
    enum CXLoadDiag_Error * error = *static_cast<enum CXLoadDiag_Error **>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: errorString (CXString *)
    CXString * errorString = nullptr;
    if (!info[2].IsNull() && !info[2].IsUndefined() && info[2].IsObject()) {
        errorString = static_cast<CXString *>(unwrapPointer(info[2].As<Napi::Object>()));
    }
    
    CXDiagnosticSet result = clang_loadDiagnostics(file, error, errorString);
    CXDiagnosticSet* resultPtr = new CXDiagnosticSet;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXDiagnosticSet");
}

// Returns non-zero if the given source location is in the main file of
// the corresponding translation unit.
static Napi::Value clang_Location_isFromMainFile_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: location (CXSourceLocation)
    CXSourceLocation location = *static_cast<CXSourceLocation*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Location_isFromMainFile(location);
    return Napi::Number::New(env, result);
}

// Returns non-zero if the given source location is in a system header.
static Napi::Value clang_Location_isInSystemHeader_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: location (CXSourceLocation)
    CXSourceLocation location = *static_cast<CXSourceLocation*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Location_isInSystemHeader(location);
    return Napi::Number::New(env, result);
}

// \param Module a module object.
// 
// \returns the module file where the provided module object came from.
static Napi::Value clang_Module_getASTFile_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Module (CXModule)
    CXModule Module = *static_cast<CXModule*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXFile result = clang_Module_getASTFile(Module);
    CXFile* resultPtr = new CXFile;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXFile");
}

// \param Module a module object.
// 
// \returns the full name of the module, e.g. "std.vector".
static Napi::Value clang_Module_getFullName_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Module (CXModule)
    CXModule Module = *static_cast<CXModule*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_Module_getFullName(Module);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// \param Module a module object.
// 
// \returns the name of the module, e.g. for the 'std.vector' sub-module it
// will return "vector".
static Napi::Value clang_Module_getName_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Module (CXModule)
    CXModule Module = *static_cast<CXModule*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_Module_getName(Module);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// \param Module a module object.
// 
// \returns the number of top level headers associated with this module.
static Napi::Value clang_Module_getNumTopLevelHeaders_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXTranslationUnit)
    CXTranslationUnit arg0 = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: Module (CXModule)
    CXModule Module = *static_cast<CXModule*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    auto result = clang_Module_getNumTopLevelHeaders(arg0, Module);
    return Napi::Number::New(env, result);
}

// \param Module a module object.
// 
// \returns the parent of a sub-module or NULL if the given module is top-level,
// e.g. for 'std.vector' it will return the 'std' module.
static Napi::Value clang_Module_getParent_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Module (CXModule)
    CXModule Module = *static_cast<CXModule*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXModule result = clang_Module_getParent(Module);
    CXModule* resultPtr = new CXModule;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXModule");
}

// \param Module a module object.
// 
// \param Index top level header index (zero-based).
// 
// \returns the specified top level header associated with the module.
static Napi::Value clang_Module_getTopLevelHeader_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXTranslationUnit)
    CXTranslationUnit arg0 = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: Module (CXModule)
    CXModule Module = *static_cast<CXModule*>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: Index (unsigned int)
    auto Index = info[2].As<Napi::Number>().Uint32Value();
    
    CXFile result = clang_Module_getTopLevelHeader(arg0, Module, Index);
    CXFile* resultPtr = new CXFile;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXFile");
}

// \param Module a module object.
// 
// \returns non-zero if the module is a system one.
static Napi::Value clang_Module_isSystem_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Module (CXModule)
    CXModule Module = *static_cast<CXModule*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Module_isSystem(Module);
    return Napi::Number::New(env, result);
}

// Create a \c CXModuleMapDescriptor object.
// Must be disposed with \c clang_ModuleMapDescriptor_dispose().
// 
// \param options is reserved, always pass 0.
static Napi::Value clang_ModuleMapDescriptor_create_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: options (unsigned int)
    auto options = info[0].As<Napi::Number>().Uint32Value();
    
    CXModuleMapDescriptor result = clang_ModuleMapDescriptor_create(options);
    CXModuleMapDescriptor* resultPtr = new CXModuleMapDescriptor;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXModuleMapDescriptor");
}

// Dispose a \c CXModuleMapDescriptor object.
static Napi::Value clang_ModuleMapDescriptor_dispose_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXModuleMapDescriptor)
    CXModuleMapDescriptor arg0 = *static_cast<CXModuleMapDescriptor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    clang_ModuleMapDescriptor_dispose(arg0);
    return env.Undefined();
}

// Sets the framework module name that the module.modulemap describes.
// \returns 0 for success, non-zero to indicate an error.
static Napi::Value clang_ModuleMapDescriptor_setFrameworkModuleName_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXModuleMapDescriptor)
    CXModuleMapDescriptor arg0 = *static_cast<CXModuleMapDescriptor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: name (const char *)
    std::string name_str = info[1].As<Napi::String>().Utf8Value();
    const char* name = name_str.c_str();
    
    auto result = clang_ModuleMapDescriptor_setFrameworkModuleName(arg0, name);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Sets the umbrella header name that the module.modulemap describes.
// \returns 0 for success, non-zero to indicate an error.
static Napi::Value clang_ModuleMapDescriptor_setUmbrellaHeader_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXModuleMapDescriptor)
    CXModuleMapDescriptor arg0 = *static_cast<CXModuleMapDescriptor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: name (const char *)
    std::string name_str = info[1].As<Napi::String>().Utf8Value();
    const char* name = name_str.c_str();
    
    auto result = clang_ModuleMapDescriptor_setUmbrellaHeader(arg0, name);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Write out the \c CXModuleMapDescriptor object to a char buffer.
// 
// \param options is reserved, always pass 0.
// \param out_buffer_ptr pointer to receive the buffer pointer, which should be
// disposed using \c clang_free().
// \param out_buffer_size pointer to receive the buffer size.
// \returns 0 for success, non-zero to indicate an error.
static Napi::Value clang_ModuleMapDescriptor_writeToBuffer_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 4) {
        Napi::TypeError::New(env, "Expected 4 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXModuleMapDescriptor)
    CXModuleMapDescriptor arg0 = *static_cast<CXModuleMapDescriptor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: options (unsigned int)
    auto options = info[1].As<Napi::Number>().Uint32Value();
    // Parameter: out_buffer_ptr (char **)
    char ** out_buffer_ptr = nullptr;
    std::vector<std::string> out_buffer_ptr_strings;
    std::vector<char*> out_buffer_ptr_cstrs;
    if (!info[2].IsNull() && !info[2].IsUndefined()) {
        if (info[2].IsArray()) {
            Napi::Array arr = info[2].As<Napi::Array>();
            for (uint32_t j = 0; j < arr.Length(); j++) {
                out_buffer_ptr_strings.push_back(arr.Get(j).As<Napi::String>().Utf8Value());
            }
            for (auto& s : out_buffer_ptr_strings) {
                out_buffer_ptr_cstrs.push_back(const_cast<char*>(s.c_str()));
            }
            out_buffer_ptr = out_buffer_ptr_cstrs.data();
        } else if (info[2].IsObject()) {
            // Assume it's a wrapped pointer
            out_buffer_ptr = *static_cast<char ***>(unwrapPointer(info[2].As<Napi::Object>()));
        }
    }
    // Parameter: out_buffer_size (unsigned int *)
    unsigned int * out_buffer_size = *static_cast<unsigned int **>(unwrapPointer(info[3].As<Napi::Object>()));
    
    auto result = clang_ModuleMapDescriptor_writeToBuffer(arg0, options, out_buffer_ptr, out_buffer_size);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Same as \c clang_parseTranslationUnit2, but returns
// the \c CXTranslationUnit instead of an error code.  In case of an error this
// routine returns a \c NULL \c CXTranslationUnit, without further detailed
// error codes.
static Napi::Value clang_parseTranslationUnit_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 7) {
        Napi::TypeError::New(env, "Expected 7 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: CIdx (CXIndex)
    CXIndex CIdx = *static_cast<CXIndex*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: source_filename (const char *)
    std::string source_filename_str = info[1].As<Napi::String>().Utf8Value();
    const char* source_filename = source_filename_str.c_str();
    // Parameter: command_line_args (const char *const *)
    const char *const * command_line_args = nullptr;
    std::vector<std::string> command_line_args_strings;
    std::vector<const char*> command_line_args_cstrs;
    if (!info[2].IsNull() && !info[2].IsUndefined()) {
        if (info[2].IsArray()) {
            Napi::Array arr = info[2].As<Napi::Array>();
            for (uint32_t j = 0; j < arr.Length(); j++) {
                command_line_args_strings.push_back(arr.Get(j).As<Napi::String>().Utf8Value());
            }
            for (const auto& s : command_line_args_strings) {
                command_line_args_cstrs.push_back(s.c_str());
            }
            command_line_args = command_line_args_cstrs.data();
        } else if (info[2].IsObject()) {
            // Assume it's a wrapped pointer
            command_line_args = *static_cast<const char *const **>(unwrapPointer(info[2].As<Napi::Object>()));
        }
    }
    // Parameter: num_command_line_args (int)
    auto num_command_line_args = info[3].As<Napi::Number>().Int32Value();
    // Parameter: unsaved_files (struct CXUnsavedFile *)
    struct CXUnsavedFile * unsaved_files = nullptr;
    if (!info[4].IsNull() && !info[4].IsUndefined() && info[4].IsObject()) {
        unsaved_files = static_cast<struct CXUnsavedFile *>(unwrapPointer(info[4].As<Napi::Object>()));
    }
    // Parameter: num_unsaved_files (unsigned int)
    auto num_unsaved_files = info[5].As<Napi::Number>().Uint32Value();
    // Parameter: options (unsigned int)
    auto options = info[6].As<Napi::Number>().Uint32Value();
    
    CXTranslationUnit result = clang_parseTranslationUnit(CIdx, source_filename, command_line_args, num_command_line_args, unsaved_files, num_unsaved_files, options);
    CXTranslationUnit* resultPtr = new CXTranslationUnit;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXTranslationUnit");
}

// Parse the given source file and the translation unit corresponding
// to that file.
// 
// This routine is the main entry point for the Clang C API, providing the
// ability to parse a source file into a translation unit that can then be
// queried by other functions in the API. This routine accepts a set of
// command-line arguments so that the compilation can be configured in the same
// way that the compiler is configured on the command line.
// 
// \param CIdx The index object with which the translation unit will be
// associated.
// 
// \param source_filename The name of the source file to load, or NULL if the
// source file is included in \c command_line_args.
// 
// \param command_line_args The command-line arguments that would be
// passed to the \c clang executable if it were being invoked out-of-process.
// These command-line options will be parsed and will affect how the translation
// unit is parsed. Note that the following options are ignored: '-c',
// '-emit-ast', '-fsyntax-only' (which is the default), and '-o \<output file>'.
// 
// \param num_command_line_args The number of command-line arguments in
// \c command_line_args.
// 
// \param unsaved_files the files that have not yet been saved to disk
// but may be required for parsing, including the contents of
// those files.  The contents and name of these files (as specified by
// CXUnsavedFile) are copied when necessary, so the client only needs to
// guarantee their validity until the call to this function returns.
// 
// \param num_unsaved_files the number of unsaved file entries in \p
// unsaved_files.
// 
// \param options A bitmask of options that affects how the translation unit
// is managed but not its compilation. This should be a bitwise OR of the
// CXTranslationUnit_XXX flags.
// 
// \param[out] out_TU A non-NULL pointer to store the created
// \c CXTranslationUnit, describing the parsed code and containing any
// diagnostics produced by the compiler.
// 
// \returns Zero on success, otherwise returns an error code.
static Napi::Value clang_parseTranslationUnit2_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 8) {
        Napi::TypeError::New(env, "Expected 8 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: CIdx (CXIndex)
    CXIndex CIdx = *static_cast<CXIndex*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: source_filename (const char *)
    std::string source_filename_str = info[1].As<Napi::String>().Utf8Value();
    const char* source_filename = source_filename_str.c_str();
    // Parameter: command_line_args (const char *const *)
    const char *const * command_line_args = nullptr;
    std::vector<std::string> command_line_args_strings;
    std::vector<const char*> command_line_args_cstrs;
    if (!info[2].IsNull() && !info[2].IsUndefined()) {
        if (info[2].IsArray()) {
            Napi::Array arr = info[2].As<Napi::Array>();
            for (uint32_t j = 0; j < arr.Length(); j++) {
                command_line_args_strings.push_back(arr.Get(j).As<Napi::String>().Utf8Value());
            }
            for (const auto& s : command_line_args_strings) {
                command_line_args_cstrs.push_back(s.c_str());
            }
            command_line_args = command_line_args_cstrs.data();
        } else if (info[2].IsObject()) {
            // Assume it's a wrapped pointer
            command_line_args = *static_cast<const char *const **>(unwrapPointer(info[2].As<Napi::Object>()));
        }
    }
    // Parameter: num_command_line_args (int)
    auto num_command_line_args = info[3].As<Napi::Number>().Int32Value();
    // Parameter: unsaved_files (struct CXUnsavedFile *)
    struct CXUnsavedFile * unsaved_files = nullptr;
    if (!info[4].IsNull() && !info[4].IsUndefined() && info[4].IsObject()) {
        unsaved_files = static_cast<struct CXUnsavedFile *>(unwrapPointer(info[4].As<Napi::Object>()));
    }
    // Parameter: num_unsaved_files (unsigned int)
    auto num_unsaved_files = info[5].As<Napi::Number>().Uint32Value();
    // Parameter: options (unsigned int)
    auto options = info[6].As<Napi::Number>().Uint32Value();
    // Parameter: out_TU (CXTranslationUnit *)
    CXTranslationUnit * out_TU = *static_cast<CXTranslationUnit **>(unwrapPointer(info[7].As<Napi::Object>()));
    
    auto result = clang_parseTranslationUnit2(CIdx, source_filename, command_line_args, num_command_line_args, unsaved_files, num_unsaved_files, options, out_TU);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Same as clang_parseTranslationUnit2 but requires a full command line
// for \c command_line_args including argv[0]. This is useful if the standard
// library paths are relative to the binary.
static Napi::Value clang_parseTranslationUnit2FullArgv_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 8) {
        Napi::TypeError::New(env, "Expected 8 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: CIdx (CXIndex)
    CXIndex CIdx = *static_cast<CXIndex*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: source_filename (const char *)
    std::string source_filename_str = info[1].As<Napi::String>().Utf8Value();
    const char* source_filename = source_filename_str.c_str();
    // Parameter: command_line_args (const char *const *)
    const char *const * command_line_args = nullptr;
    std::vector<std::string> command_line_args_strings;
    std::vector<const char*> command_line_args_cstrs;
    if (!info[2].IsNull() && !info[2].IsUndefined()) {
        if (info[2].IsArray()) {
            Napi::Array arr = info[2].As<Napi::Array>();
            for (uint32_t j = 0; j < arr.Length(); j++) {
                command_line_args_strings.push_back(arr.Get(j).As<Napi::String>().Utf8Value());
            }
            for (const auto& s : command_line_args_strings) {
                command_line_args_cstrs.push_back(s.c_str());
            }
            command_line_args = command_line_args_cstrs.data();
        } else if (info[2].IsObject()) {
            // Assume it's a wrapped pointer
            command_line_args = *static_cast<const char *const **>(unwrapPointer(info[2].As<Napi::Object>()));
        }
    }
    // Parameter: num_command_line_args (int)
    auto num_command_line_args = info[3].As<Napi::Number>().Int32Value();
    // Parameter: unsaved_files (struct CXUnsavedFile *)
    struct CXUnsavedFile * unsaved_files = nullptr;
    if (!info[4].IsNull() && !info[4].IsUndefined() && info[4].IsObject()) {
        unsaved_files = static_cast<struct CXUnsavedFile *>(unwrapPointer(info[4].As<Napi::Object>()));
    }
    // Parameter: num_unsaved_files (unsigned int)
    auto num_unsaved_files = info[5].As<Napi::Number>().Uint32Value();
    // Parameter: options (unsigned int)
    auto options = info[6].As<Napi::Number>().Uint32Value();
    // Parameter: out_TU (CXTranslationUnit *)
    CXTranslationUnit * out_TU = *static_cast<CXTranslationUnit **>(unwrapPointer(info[7].As<Napi::Object>()));
    
    auto result = clang_parseTranslationUnit2FullArgv(CIdx, source_filename, command_line_args, num_command_line_args, unsaved_files, num_unsaved_files, options, out_TU);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Release a printing policy.
static Napi::Value clang_PrintingPolicy_dispose_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Policy (CXPrintingPolicy)
    CXPrintingPolicy Policy = *static_cast<CXPrintingPolicy*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    clang_PrintingPolicy_dispose(Policy);
    return env.Undefined();
}

// Get a property value for the given printing policy.
static Napi::Value clang_PrintingPolicy_getProperty_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Policy (CXPrintingPolicy)
    CXPrintingPolicy Policy = *static_cast<CXPrintingPolicy*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: Property (enum CXPrintingPolicyProperty)
    enum CXPrintingPolicyProperty Property = static_cast<enum CXPrintingPolicyProperty>(info[1].As<Napi::Number>().Int32Value());
    
    auto result = clang_PrintingPolicy_getProperty(Policy, Property);
    return Napi::Number::New(env, result);
}

// Set a property value for the given printing policy.
static Napi::Value clang_PrintingPolicy_setProperty_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Policy (CXPrintingPolicy)
    CXPrintingPolicy Policy = *static_cast<CXPrintingPolicy*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: Property (enum CXPrintingPolicyProperty)
    enum CXPrintingPolicyProperty Property = static_cast<enum CXPrintingPolicyProperty>(info[1].As<Napi::Number>().Int32Value());
    // Parameter: Value (unsigned int)
    auto Value = info[2].As<Napi::Number>().Uint32Value();
    
    clang_PrintingPolicy_setProperty(Policy, Property, Value);
    return env.Undefined();
}

// Returns non-zero if \p range is null.
static Napi::Value clang_Range_isNull_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: range (CXSourceRange)
    CXSourceRange range = *static_cast<CXSourceRange*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Range_isNull(range);
    return Napi::Number::New(env, result);
}

// Dispose the remapping.
static Napi::Value clang_remap_dispose_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXRemapping)
    CXRemapping arg0 = *static_cast<CXRemapping*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    clang_remap_dispose(arg0);
    return env.Undefined();
}

// Get the original and the associated filename from the remapping.
// 
// \param original If non-NULL, will be set to the original filename.
// 
// \param transformed If non-NULL, will be set to the filename that the original
// is associated with.
static Napi::Value clang_remap_getFilenames_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 4) {
        Napi::TypeError::New(env, "Expected 4 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXRemapping)
    CXRemapping arg0 = *static_cast<CXRemapping*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: index (unsigned int)
    auto index = info[1].As<Napi::Number>().Uint32Value();
    // Parameter: original (CXString *)
    CXString * original = nullptr;
    if (!info[2].IsNull() && !info[2].IsUndefined() && info[2].IsObject()) {
        original = static_cast<CXString *>(unwrapPointer(info[2].As<Napi::Object>()));
    }
    // Parameter: transformed (CXString *)
    CXString * transformed = nullptr;
    if (!info[3].IsNull() && !info[3].IsUndefined() && info[3].IsObject()) {
        transformed = static_cast<CXString *>(unwrapPointer(info[3].As<Napi::Object>()));
    }
    
    clang_remap_getFilenames(arg0, index, original, transformed);
    return env.Undefined();
}

// Determine the number of remappings.
static Napi::Value clang_remap_getNumFiles_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXRemapping)
    CXRemapping arg0 = *static_cast<CXRemapping*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_remap_getNumFiles(arg0);
    return Napi::Number::New(env, result);
}

// Reparse the source files that produced this translation unit.
// 
// This routine can be used to re-parse the source files that originally
// created the given translation unit, for example because those source files
// have changed (either on disk or as passed via \p unsaved_files). The
// source code will be reparsed with the same command-line options as it
// was originally parsed.
// 
// Reparsing a translation unit invalidates all cursors and source locations
// that refer into that translation unit. This makes reparsing a translation
// unit semantically equivalent to destroying the translation unit and then
// creating a new translation unit with the same command-line arguments.
// However, it may be more efficient to reparse a translation
// unit using this routine.
// 
// \param TU The translation unit whose contents will be re-parsed. The
// translation unit must originally have been built with
// \c clang_createTranslationUnitFromSourceFile().
// 
// \param num_unsaved_files The number of unsaved file entries in \p
// unsaved_files.
// 
// \param unsaved_files The files that have not yet been saved to disk
// but may be required for parsing, including the contents of
// those files.  The contents and name of these files (as specified by
// CXUnsavedFile) are copied when necessary, so the client only needs to
// guarantee their validity until the call to this function returns.
// 
// \param options A bitset of options composed of the flags in CXReparse_Flags.
// The function \c clang_defaultReparseOptions() produces a default set of
// options recommended for most uses, based on the translation unit.
// 
// \returns 0 if the sources could be reparsed.  A non-zero error code will be
// returned if reparsing was impossible, such that the translation unit is
// invalid. In such cases, the only valid call for \c TU is
// \c clang_disposeTranslationUnit(TU).  The error codes returned by this
// routine are described by the \c CXErrorCode enum.
static Napi::Value clang_reparseTranslationUnit_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 4) {
        Napi::TypeError::New(env, "Expected 4 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: TU (CXTranslationUnit)
    CXTranslationUnit TU = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: num_unsaved_files (unsigned int)
    auto num_unsaved_files = info[1].As<Napi::Number>().Uint32Value();
    // Parameter: unsaved_files (struct CXUnsavedFile *)
    struct CXUnsavedFile * unsaved_files = nullptr;
    if (!info[2].IsNull() && !info[2].IsUndefined() && info[2].IsObject()) {
        unsaved_files = static_cast<struct CXUnsavedFile *>(unwrapPointer(info[2].As<Napi::Object>()));
    }
    // Parameter: options (unsigned int)
    auto options = info[3].As<Napi::Number>().Uint32Value();
    
    auto result = clang_reparseTranslationUnit(TU, num_unsaved_files, unsaved_files, options);
    return Napi::Number::New(env, result);
}

// Saves a translation unit into a serialized representation of
// that translation unit on disk.
// 
// Any translation unit that was parsed without error can be saved
// into a file. The translation unit can then be deserialized into a
// new \c CXTranslationUnit with \c clang_createTranslationUnit() or,
// if it is an incomplete translation unit that corresponds to a
// header, used as a precompiled header when parsing other translation
// units.
// 
// \param TU The translation unit to save.
// 
// \param FileName The file to which the translation unit will be saved.
// 
// \param options A bitmask of options that affects how the translation unit
// is saved. This should be a bitwise OR of the
// CXSaveTranslationUnit_XXX flags.
// 
// \returns A value that will match one of the enumerators of the CXSaveError
// enumeration. Zero (CXSaveError_None) indicates that the translation unit was
// saved successfully, while a non-zero value indicates that a problem occurred.
static Napi::Value clang_saveTranslationUnit_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: TU (CXTranslationUnit)
    CXTranslationUnit TU = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: FileName (const char *)
    std::string FileName_str = info[1].As<Napi::String>().Utf8Value();
    const char* FileName = FileName_str.c_str();
    // Parameter: options (unsigned int)
    auto options = info[2].As<Napi::Number>().Uint32Value();
    
    auto result = clang_saveTranslationUnit(TU, FileName, options);
    return Napi::Number::New(env, result);
}

// Sort the code-completion results in case-insensitive alphabetical
// order.
// 
// \param Results The set of results to sort.
// \param NumResults The number of results in \p Results.
static Napi::Value clang_sortCodeCompletionResults_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Results (CXCompletionResult *)
    CXCompletionResult * Results = nullptr;
    if (!info[0].IsNull() && !info[0].IsUndefined() && info[0].IsObject()) {
        Results = static_cast<CXCompletionResult *>(unwrapPointer(info[0].As<Napi::Object>()));
    }
    // Parameter: NumResults (unsigned int)
    auto NumResults = info[1].As<Napi::Number>().Uint32Value();
    
    clang_sortCodeCompletionResults(Results, NumResults);
    return env.Undefined();
}

// Suspend a translation unit in order to free memory associated with it.
// 
// A suspended translation unit uses significantly less memory but on the other
// side does not support any other calls than \c clang_reparseTranslationUnit
// to resume it or \c clang_disposeTranslationUnit to dispose it completely.
static Napi::Value clang_suspendTranslationUnit_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXTranslationUnit)
    CXTranslationUnit arg0 = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_suspendTranslationUnit(arg0);
    return Napi::Number::New(env, result);
}

// Destroy the CXTargetInfo object.
static Napi::Value clang_TargetInfo_dispose_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Info (CXTargetInfo)
    CXTargetInfo Info = *static_cast<CXTargetInfo*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    clang_TargetInfo_dispose(Info);
    return env.Undefined();
}

// Get the pointer width of the target in bits.
// 
// Returns -1 in case of error.
static Napi::Value clang_TargetInfo_getPointerWidth_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Info (CXTargetInfo)
    CXTargetInfo Info = *static_cast<CXTargetInfo*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_TargetInfo_getPointerWidth(Info);
    return Napi::Number::New(env, result);
}

// Get the normalized target triple as a string.
// 
// Returns the empty string in case of any error.
static Napi::Value clang_TargetInfo_getTriple_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: Info (CXTargetInfo)
    CXTargetInfo Info = *static_cast<CXTargetInfo*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_TargetInfo_getTriple(Info);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Enable/disable crash recovery.
// 
// \param isEnabled Flag to indicate if crash recovery is enabled.  A non-zero
// value enables crash recovery, while 0 disables it.
static Napi::Value clang_toggleCrashRecovery_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: isEnabled (unsigned int)
    auto isEnabled = info[0].As<Napi::Number>().Uint32Value();
    
    clang_toggleCrashRecovery(isEnabled);
    return env.Undefined();
}

// Tokenize the source code described by the given range into raw
// lexical tokens.
// 
// \param TU the translation unit whose text is being tokenized.
// 
// \param Range the source range in which text should be tokenized. All of the
// tokens produced by tokenization will fall within this source range,
// 
// \param Tokens this pointer will be set to point to the array of tokens
// that occur within the given source range. The returned pointer must be
// freed with clang_disposeTokens() before the translation unit is destroyed.
// 
// \param NumTokens will be set to the number of tokens in the \c *Tokens
// array.
static Napi::Value clang_tokenize_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 4) {
        Napi::TypeError::New(env, "Expected 4 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: TU (CXTranslationUnit)
    CXTranslationUnit TU = *static_cast<CXTranslationUnit*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: Range (CXSourceRange)
    CXSourceRange Range = *static_cast<CXSourceRange*>(unwrapPointer(info[1].As<Napi::Object>()));
    // Parameter: Tokens (CXToken **)
    CXToken ** Tokens = *static_cast<CXToken ***>(unwrapPointer(info[2].As<Napi::Object>()));
    // Parameter: NumTokens (unsigned int *)
    unsigned int * NumTokens = *static_cast<unsigned int **>(unwrapPointer(info[3].As<Napi::Object>()));
    
    clang_tokenize(TU, Range, Tokens, NumTokens);
    return env.Undefined();
}

// Return the alignment of a type in bytes as per C++[expr.alignof]
// standard.
// 
// If the type declaration is invalid, CXTypeLayoutError_Invalid is returned.
// If the type declaration is an incomplete type, CXTypeLayoutError_Incomplete
// is returned.
// If the type declaration is a dependent type, CXTypeLayoutError_Dependent is
// returned.
// If the type declaration is not a constant size type,
// CXTypeLayoutError_NotConstantSize is returned.
static Napi::Value clang_Type_getAlignOf_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Type_getAlignOf(T);
    return Napi::BigInt::New(env, result);
}

// Return the class type of an member pointer type.
// 
// If a non-member-pointer type is passed in, an invalid type is returned.
static Napi::Value clang_Type_getClassType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXType result = clang_Type_getClassType(T);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Retrieve the ref-qualifier kind of a function or method.
// 
// The ref-qualifier is returned for C++ functions or methods. For other types
// or non-C++ declarations, CXRefQualifier_None is returned.
static Napi::Value clang_Type_getCXXRefQualifier_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Type_getCXXRefQualifier(T);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Return the type that was modified by this attributed type.
// 
// If the type is not an attributed type, an invalid type is returned.
static Napi::Value clang_Type_getModifiedType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXType result = clang_Type_getModifiedType(T);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Retrieve the type named by the qualified-id.
// 
// If a non-elaborated type is passed in, an invalid type is returned.
static Napi::Value clang_Type_getNamedType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXType result = clang_Type_getNamedType(T);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Retrieve the nullability kind of a pointer type.
static Napi::Value clang_Type_getNullability_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Type_getNullability(T);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Retrieve the number of protocol references associated with an ObjC object/id.
// 
// If the type is not an ObjC object, 0 is returned.
static Napi::Value clang_Type_getNumObjCProtocolRefs_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Type_getNumObjCProtocolRefs(T);
    return Napi::Number::New(env, result);
}

// Retrieve the number of type arguments associated with an ObjC object.
// 
// If the type is not an ObjC object, 0 is returned.
static Napi::Value clang_Type_getNumObjCTypeArgs_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Type_getNumObjCTypeArgs(T);
    return Napi::Number::New(env, result);
}

// Returns the number of template arguments for given template
// specialization, or -1 if type \c T is not a template specialization.
static Napi::Value clang_Type_getNumTemplateArguments_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Type_getNumTemplateArguments(T);
    return Napi::Number::New(env, result);
}

// Returns the Objective-C type encoding for the specified CXType.
static Napi::Value clang_Type_getObjCEncoding_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: type (CXType)
    CXType type = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXString result = clang_Type_getObjCEncoding(type);
    CXString* resultPtr = new CXString(result);
    return wrapOwnedPointer(env, resultPtr, "CXString");
}

// Retrieves the base type of the ObjCObjectType.
// 
// If the type is not an ObjC object, an invalid type is returned.
static Napi::Value clang_Type_getObjCObjectBaseType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXType result = clang_Type_getObjCObjectBaseType(T);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Retrieve the decl for a protocol reference for an ObjC object/id.
// 
// If the type is not an ObjC object or there are not enough protocol
// references, an invalid cursor is returned.
static Napi::Value clang_Type_getObjCProtocolDecl_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: i (unsigned int)
    auto i = info[1].As<Napi::Number>().Uint32Value();
    
    CXCursor result = clang_Type_getObjCProtocolDecl(T, i);
    CXCursor* resultPtr = new CXCursor(result);
    return wrapOwnedPointer(env, resultPtr, "CXCursor");
}

// Retrieve a type argument associated with an ObjC object.
// 
// If the type is not an ObjC or the index is not valid,
// an invalid type is returned.
static Napi::Value clang_Type_getObjCTypeArg_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: i (unsigned int)
    auto i = info[1].As<Napi::Number>().Uint32Value();
    
    CXType result = clang_Type_getObjCTypeArg(T, i);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Return the offset of a field named S in a record of type T in bits
// as it would be returned by __offsetof__ as per C++11[18.2p4]
// 
// If the cursor is not a record field declaration, CXTypeLayoutError_Invalid
// is returned.
// If the field's type declaration is an incomplete type,
// CXTypeLayoutError_Incomplete is returned.
// If the field's type declaration is a dependent type,
// CXTypeLayoutError_Dependent is returned.
// If the field's name S is not found,
// CXTypeLayoutError_InvalidFieldName is returned.
static Napi::Value clang_Type_getOffsetOf_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: S (const char *)
    std::string S_str = info[1].As<Napi::String>().Utf8Value();
    const char* S = S_str.c_str();
    
    auto result = clang_Type_getOffsetOf(T, S);
    return Napi::BigInt::New(env, result);
}

// Return the size of a type in bytes as per C++[expr.sizeof] standard.
// 
// If the type declaration is invalid, CXTypeLayoutError_Invalid is returned.
// If the type declaration is an incomplete type, CXTypeLayoutError_Incomplete
// is returned.
// If the type declaration is a dependent type, CXTypeLayoutError_Dependent is
// returned.
static Napi::Value clang_Type_getSizeOf_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Type_getSizeOf(T);
    return Napi::BigInt::New(env, result);
}

// Returns the type template argument of a template class specialization
// at given index.
// 
// This function only returns template type arguments and does not handle
// template template arguments or variadic packs.
static Napi::Value clang_Type_getTemplateArgumentAsType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: i (unsigned int)
    auto i = info[1].As<Napi::Number>().Uint32Value();
    
    CXType result = clang_Type_getTemplateArgumentAsType(T, i);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Gets the type contained by this atomic type.
// 
// If a non-atomic type is passed in, an invalid type is returned.
static Napi::Value clang_Type_getValueType_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: CT (CXType)
    CXType CT = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    CXType result = clang_Type_getValueType(CT);
    CXType* resultPtr = new CXType(result);
    return wrapOwnedPointer(env, resultPtr, "CXType");
}

// Determine if a typedef is 'transparent' tag.
// 
// A typedef is considered 'transparent' if it shares a name and spelling
// location with its underlying tag type, as is the case with the NS_ENUM macro.
// 
// \returns non-zero if transparent and zero otherwise.
static Napi::Value clang_Type_isTransparentTagTypedef_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    auto result = clang_Type_isTransparentTagTypedef(T);
    return Napi::Number::New(env, result);
}

// Visit the fields of a particular type.
// 
// This function visits all the direct fields of the given cursor,
// invoking the given \p visitor function with the cursors of each
// visited field. The traversal may be ended prematurely, if
// the visitor returns \c CXFieldVisit_Break.
// 
// \param T the record type whose field may be visited.
// 
// \param visitor the visitor function that will be invoked for each
// field of \p T.
// 
// \param client_data pointer data supplied by the client, which will
// be passed to the visitor each time it is invoked.
// 
// \returns a non-zero value if the traversal was terminated
// prematurely by the visitor returning \c CXFieldVisit_Break.
static Napi::Value clang_Type_visitFields_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: visitor (CXFieldVisitor)
    // Parameter: visitor (CXFieldVisitor)
    CXFieldVisitor visitor = nullptr;
    if (info[1].IsExternal()) {
        visitor = reinterpret_cast<CXFieldVisitor>(info[1].As<Napi::External<void>>().Data());
    }
    // Parameter: client_data (CXClientData)
    CXClientData client_data = *static_cast<CXClientData*>(unwrapPointer(info[2].As<Napi::Object>()));
    
    auto result = clang_Type_visitFields(T, visitor, client_data);
    return Napi::Number::New(env, result);
}

// Map an absolute virtual file path to an absolute real one.
// The virtual path must be canonicalized (not contain "."/"..").
// \returns 0 for success, non-zero to indicate an error.
static Napi::Value clang_VirtualFileOverlay_addFileMapping_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXVirtualFileOverlay)
    CXVirtualFileOverlay arg0 = *static_cast<CXVirtualFileOverlay*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: virtualPath (const char *)
    std::string virtualPath_str = info[1].As<Napi::String>().Utf8Value();
    const char* virtualPath = virtualPath_str.c_str();
    // Parameter: realPath (const char *)
    std::string realPath_str = info[2].As<Napi::String>().Utf8Value();
    const char* realPath = realPath_str.c_str();
    
    auto result = clang_VirtualFileOverlay_addFileMapping(arg0, virtualPath, realPath);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Create a \c CXVirtualFileOverlay object.
// Must be disposed with \c clang_VirtualFileOverlay_dispose().
// 
// \param options is reserved, always pass 0.
static Napi::Value clang_VirtualFileOverlay_create_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: options (unsigned int)
    auto options = info[0].As<Napi::Number>().Uint32Value();
    
    CXVirtualFileOverlay result = clang_VirtualFileOverlay_create(options);
    CXVirtualFileOverlay* resultPtr = new CXVirtualFileOverlay;
    *resultPtr = result;
    return wrapOwnedPointer(env, resultPtr, "CXVirtualFileOverlay");
}

// Dispose a \c CXVirtualFileOverlay object.
static Napi::Value clang_VirtualFileOverlay_dispose_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXVirtualFileOverlay)
    CXVirtualFileOverlay arg0 = *static_cast<CXVirtualFileOverlay*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    clang_VirtualFileOverlay_dispose(arg0);
    return env.Undefined();
}

// Set the case sensitivity for the \c CXVirtualFileOverlay object.
// The \c CXVirtualFileOverlay object is case-sensitive by default, this
// option can be used to override the default.
// \returns 0 for success, non-zero to indicate an error.
static Napi::Value clang_VirtualFileOverlay_setCaseSensitivity_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXVirtualFileOverlay)
    CXVirtualFileOverlay arg0 = *static_cast<CXVirtualFileOverlay*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: caseSensitive (int)
    auto caseSensitive = info[1].As<Napi::Number>().Int32Value();
    
    auto result = clang_VirtualFileOverlay_setCaseSensitivity(arg0, caseSensitive);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Write out the \c CXVirtualFileOverlay object to a char buffer.
// 
// \param options is reserved, always pass 0.
// \param out_buffer_ptr pointer to receive the buffer pointer, which should be
// disposed using \c clang_free().
// \param out_buffer_size pointer to receive the buffer size.
// \returns 0 for success, non-zero to indicate an error.
static Napi::Value clang_VirtualFileOverlay_writeToBuffer_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 4) {
        Napi::TypeError::New(env, "Expected 4 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: arg0 (CXVirtualFileOverlay)
    CXVirtualFileOverlay arg0 = *static_cast<CXVirtualFileOverlay*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: options (unsigned int)
    auto options = info[1].As<Napi::Number>().Uint32Value();
    // Parameter: out_buffer_ptr (char **)
    char ** out_buffer_ptr = nullptr;
    std::vector<std::string> out_buffer_ptr_strings;
    std::vector<char*> out_buffer_ptr_cstrs;
    if (!info[2].IsNull() && !info[2].IsUndefined()) {
        if (info[2].IsArray()) {
            Napi::Array arr = info[2].As<Napi::Array>();
            for (uint32_t j = 0; j < arr.Length(); j++) {
                out_buffer_ptr_strings.push_back(arr.Get(j).As<Napi::String>().Utf8Value());
            }
            for (auto& s : out_buffer_ptr_strings) {
                out_buffer_ptr_cstrs.push_back(const_cast<char*>(s.c_str()));
            }
            out_buffer_ptr = out_buffer_ptr_cstrs.data();
        } else if (info[2].IsObject()) {
            // Assume it's a wrapped pointer
            out_buffer_ptr = *static_cast<char ***>(unwrapPointer(info[2].As<Napi::Object>()));
        }
    }
    // Parameter: out_buffer_size (unsigned int *)
    unsigned int * out_buffer_size = *static_cast<unsigned int **>(unwrapPointer(info[3].As<Napi::Object>()));
    
    auto result = clang_VirtualFileOverlay_writeToBuffer(arg0, options, out_buffer_ptr, out_buffer_size);
    return Napi::Number::New(env, static_cast<int>(result));
}

// Visit the children of a particular cursor.
// 
// This function visits all the direct children of the given cursor,
// invoking the given \p visitor function with the cursors of each
// visited child. The traversal may be recursive, if the visitor returns
// \c CXChildVisit_Recurse. The traversal may also be ended prematurely, if
// the visitor returns \c CXChildVisit_Break.
// 
// \param parent the cursor whose child may be visited. All kinds of
// cursors can be visited, including invalid cursors (which, by
// definition, have no children).
// 
// \param visitor the visitor function that will be invoked for each
// child of \p parent.
// 
// \param client_data pointer data supplied by the client, which will
// be passed to the visitor each time it is invoked.
// 
// \returns a non-zero value if the traversal was terminated
// prematurely by the visitor returning \c CXChildVisit_Break.
static Napi::Value clang_visitChildren_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: parent (CXCursor)
    CXCursor parent = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    
    // Parameter: visitor (JavaScript callback function)
    if (!info[1].IsFunction()) {
        Napi::TypeError::New(env, "Expected function for visitor parameter").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    Napi::Function jsVisitor = info[1].As<Napi::Function>();
    
    // Create C++ visitor function that forwards to JS
    struct VisitorThunkData { Napi::FunctionReference fnRef; };
    VisitorThunkData data{ Napi::Persistent(jsVisitor) };
    auto visitor = [](CXCursor cursor, CXCursor parent, CXClientData client_data) -> enum CXChildVisitResult {
        auto* d = static_cast<VisitorThunkData*>(client_data);
        Napi::Env env = d->fnRef.Env();
        CXCursor* cursorPtr = new CXCursor(cursor);
        Napi::Object cursorObj = wrapOwnedPointer(env, cursorPtr, "CXCursor");
        CXCursor* parentPtr = new CXCursor(parent);
        Napi::Object parentObj = wrapOwnedPointer(env, parentPtr, "CXCursor");
        Napi::Value result = d->fnRef.Call({cursorObj, parentObj});
        if (result.IsNumber()) {
            return static_cast<enum CXChildVisitResult>(result.As<Napi::Number>().Int32Value());
        }
        return CXChildVisit_Break;
    };
    auto result = clang_visitChildren(parent, visitor, &data);
    return Napi::Number::New(env, result);
}

// Visits the children of a cursor using the specified block.  Behaves
// identically to clang_visitChildren() in all other respects.
static Napi::Value clang_visitChildrenWithBlock_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected 2 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: parent (CXCursor)
    CXCursor parent = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: block (CXCursorVisitorBlock)
    CXCursorVisitorBlock block = *static_cast<CXCursorVisitorBlock*>(unwrapPointer(info[1].As<Napi::Object>()));
    
    auto result = clang_visitChildrenWithBlock(parent, block);
    return Napi::Number::New(env, result);
}

// Visit the base classes of a type.
// 
// This function visits all the direct base classes of a the given cursor,
// invoking the given \p visitor function with the cursors of each
// visited base. The traversal may be ended prematurely, if
// the visitor returns \c CXFieldVisit_Break.
// 
// \param T the record type whose field may be visited.
// 
// \param visitor the visitor function that will be invoked for each
// field of \p T.
// 
// \param client_data pointer data supplied by the client, which will
// be passed to the visitor each time it is invoked.
// 
// \returns a non-zero value if the traversal was terminated
// prematurely by the visitor returning \c CXFieldVisit_Break.
static Napi::Value clang_visitCXXBaseClasses_wrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    
    // Parameter: T (CXType)
    CXType T = *static_cast<CXType*>(unwrapPointer(info[0].As<Napi::Object>()));
    // Parameter: visitor (CXFieldVisitor)
    // Parameter: visitor (CXFieldVisitor)
    CXFieldVisitor visitor = nullptr;
    if (info[1].IsExternal()) {
        visitor = reinterpret_cast<CXFieldVisitor>(info[1].As<Napi::External<void>>().Data());
    }
    // Parameter: client_data (CXClientData)
    CXClientData client_data = *static_cast<CXClientData*>(unwrapPointer(info[2].As<Napi::Object>()));
    
    auto result = clang_visitCXXBaseClasses(T, visitor, client_data);
    return Napi::Number::New(env, result);
}


static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("CXError_Success", Napi::Number::New(env, CXError_Success));
    exports.Set("CXError_Failure", Napi::Number::New(env, CXError_Failure));
    exports.Set("CXError_Crashed", Napi::Number::New(env, CXError_Crashed));
    exports.Set("CXError_InvalidArguments", Napi::Number::New(env, CXError_InvalidArguments));
    exports.Set("CXError_ASTReadError", Napi::Number::New(env, CXError_ASTReadError));
    exports.Set("CXDiagnostic_Ignored", Napi::Number::New(env, CXDiagnostic_Ignored));
    exports.Set("CXDiagnostic_Note", Napi::Number::New(env, CXDiagnostic_Note));
    exports.Set("CXDiagnostic_Warning", Napi::Number::New(env, CXDiagnostic_Warning));
    exports.Set("CXDiagnostic_Error", Napi::Number::New(env, CXDiagnostic_Error));
    exports.Set("CXDiagnostic_Fatal", Napi::Number::New(env, CXDiagnostic_Fatal));
    exports.Set("CXLoadDiag_None", Napi::Number::New(env, CXLoadDiag_None));
    exports.Set("CXLoadDiag_Unknown", Napi::Number::New(env, CXLoadDiag_Unknown));
    exports.Set("CXLoadDiag_CannotLoad", Napi::Number::New(env, CXLoadDiag_CannotLoad));
    exports.Set("CXLoadDiag_InvalidFile", Napi::Number::New(env, CXLoadDiag_InvalidFile));
    exports.Set("CXDiagnostic_DisplaySourceLocation", Napi::Number::New(env, CXDiagnostic_DisplaySourceLocation));
    exports.Set("CXDiagnostic_DisplayColumn", Napi::Number::New(env, CXDiagnostic_DisplayColumn));
    exports.Set("CXDiagnostic_DisplaySourceRanges", Napi::Number::New(env, CXDiagnostic_DisplaySourceRanges));
    exports.Set("CXDiagnostic_DisplayOption", Napi::Number::New(env, CXDiagnostic_DisplayOption));
    exports.Set("CXDiagnostic_DisplayCategoryId", Napi::Number::New(env, CXDiagnostic_DisplayCategoryId));
    exports.Set("CXDiagnostic_DisplayCategoryName", Napi::Number::New(env, CXDiagnostic_DisplayCategoryName));
    exports.Set("CXAvailability_Available", Napi::Number::New(env, CXAvailability_Available));
    exports.Set("CXAvailability_Deprecated", Napi::Number::New(env, CXAvailability_Deprecated));
    exports.Set("CXAvailability_NotAvailable", Napi::Number::New(env, CXAvailability_NotAvailable));
    exports.Set("CXAvailability_NotAccessible", Napi::Number::New(env, CXAvailability_NotAccessible));
    exports.Set("CXCursor_ExceptionSpecificationKind_None", Napi::Number::New(env, CXCursor_ExceptionSpecificationKind_None));
    exports.Set("CXCursor_ExceptionSpecificationKind_DynamicNone", Napi::Number::New(env, CXCursor_ExceptionSpecificationKind_DynamicNone));
    exports.Set("CXCursor_ExceptionSpecificationKind_Dynamic", Napi::Number::New(env, CXCursor_ExceptionSpecificationKind_Dynamic));
    exports.Set("CXCursor_ExceptionSpecificationKind_MSAny", Napi::Number::New(env, CXCursor_ExceptionSpecificationKind_MSAny));
    exports.Set("CXCursor_ExceptionSpecificationKind_BasicNoexcept", Napi::Number::New(env, CXCursor_ExceptionSpecificationKind_BasicNoexcept));
    exports.Set("CXCursor_ExceptionSpecificationKind_ComputedNoexcept", Napi::Number::New(env, CXCursor_ExceptionSpecificationKind_ComputedNoexcept));
    exports.Set("CXCursor_ExceptionSpecificationKind_Unevaluated", Napi::Number::New(env, CXCursor_ExceptionSpecificationKind_Unevaluated));
    exports.Set("CXCursor_ExceptionSpecificationKind_Uninstantiated", Napi::Number::New(env, CXCursor_ExceptionSpecificationKind_Uninstantiated));
    exports.Set("CXCursor_ExceptionSpecificationKind_Unparsed", Napi::Number::New(env, CXCursor_ExceptionSpecificationKind_Unparsed));
    exports.Set("CXCursor_ExceptionSpecificationKind_NoThrow", Napi::Number::New(env, CXCursor_ExceptionSpecificationKind_NoThrow));
    exports.Set("CXChoice_Default", Napi::Number::New(env, CXChoice_Default));
    exports.Set("CXChoice_Enabled", Napi::Number::New(env, CXChoice_Enabled));
    exports.Set("CXChoice_Disabled", Napi::Number::New(env, CXChoice_Disabled));
    exports.Set("CXGlobalOpt_None", Napi::Number::New(env, CXGlobalOpt_None));
    exports.Set("CXGlobalOpt_ThreadBackgroundPriorityForIndexing", Napi::Number::New(env, CXGlobalOpt_ThreadBackgroundPriorityForIndexing));
    exports.Set("CXGlobalOpt_ThreadBackgroundPriorityForEditing", Napi::Number::New(env, CXGlobalOpt_ThreadBackgroundPriorityForEditing));
    exports.Set("CXGlobalOpt_ThreadBackgroundPriorityForAll", Napi::Number::New(env, CXGlobalOpt_ThreadBackgroundPriorityForAll));
    exports.Set("CXTranslationUnit_None", Napi::Number::New(env, CXTranslationUnit_None));
    exports.Set("CXTranslationUnit_DetailedPreprocessingRecord", Napi::Number::New(env, CXTranslationUnit_DetailedPreprocessingRecord));
    exports.Set("CXTranslationUnit_Incomplete", Napi::Number::New(env, CXTranslationUnit_Incomplete));
    exports.Set("CXTranslationUnit_PrecompiledPreamble", Napi::Number::New(env, CXTranslationUnit_PrecompiledPreamble));
    exports.Set("CXTranslationUnit_CacheCompletionResults", Napi::Number::New(env, CXTranslationUnit_CacheCompletionResults));
    exports.Set("CXTranslationUnit_ForSerialization", Napi::Number::New(env, CXTranslationUnit_ForSerialization));
    exports.Set("CXTranslationUnit_CXXChainedPCH", Napi::Number::New(env, CXTranslationUnit_CXXChainedPCH));
    exports.Set("CXTranslationUnit_SkipFunctionBodies", Napi::Number::New(env, CXTranslationUnit_SkipFunctionBodies));
    exports.Set("CXTranslationUnit_IncludeBriefCommentsInCodeCompletion", Napi::Number::New(env, CXTranslationUnit_IncludeBriefCommentsInCodeCompletion));
    exports.Set("CXTranslationUnit_CreatePreambleOnFirstParse", Napi::Number::New(env, CXTranslationUnit_CreatePreambleOnFirstParse));
    exports.Set("CXTranslationUnit_KeepGoing", Napi::Number::New(env, CXTranslationUnit_KeepGoing));
    exports.Set("CXTranslationUnit_SingleFileParse", Napi::Number::New(env, CXTranslationUnit_SingleFileParse));
    exports.Set("CXTranslationUnit_LimitSkipFunctionBodiesToPreamble", Napi::Number::New(env, CXTranslationUnit_LimitSkipFunctionBodiesToPreamble));
    exports.Set("CXTranslationUnit_IncludeAttributedTypes", Napi::Number::New(env, CXTranslationUnit_IncludeAttributedTypes));
    exports.Set("CXTranslationUnit_VisitImplicitAttributes", Napi::Number::New(env, CXTranslationUnit_VisitImplicitAttributes));
    exports.Set("CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles", Napi::Number::New(env, CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles));
    exports.Set("CXTranslationUnit_RetainExcludedConditionalBlocks", Napi::Number::New(env, CXTranslationUnit_RetainExcludedConditionalBlocks));
    exports.Set("CXSaveTranslationUnit_None", Napi::Number::New(env, CXSaveTranslationUnit_None));
    exports.Set("CXSaveError_None", Napi::Number::New(env, CXSaveError_None));
    exports.Set("CXSaveError_Unknown", Napi::Number::New(env, CXSaveError_Unknown));
    exports.Set("CXSaveError_TranslationErrors", Napi::Number::New(env, CXSaveError_TranslationErrors));
    exports.Set("CXSaveError_InvalidTU", Napi::Number::New(env, CXSaveError_InvalidTU));
    exports.Set("CXReparse_None", Napi::Number::New(env, CXReparse_None));
    exports.Set("CXTUResourceUsage_AST", Napi::Number::New(env, CXTUResourceUsage_AST));
    exports.Set("CXTUResourceUsage_Identifiers", Napi::Number::New(env, CXTUResourceUsage_Identifiers));
    exports.Set("CXTUResourceUsage_Selectors", Napi::Number::New(env, CXTUResourceUsage_Selectors));
    exports.Set("CXTUResourceUsage_GlobalCompletionResults", Napi::Number::New(env, CXTUResourceUsage_GlobalCompletionResults));
    exports.Set("CXTUResourceUsage_SourceManagerContentCache", Napi::Number::New(env, CXTUResourceUsage_SourceManagerContentCache));
    exports.Set("CXTUResourceUsage_AST_SideTables", Napi::Number::New(env, CXTUResourceUsage_AST_SideTables));
    exports.Set("CXTUResourceUsage_SourceManager_Membuffer_Malloc", Napi::Number::New(env, CXTUResourceUsage_SourceManager_Membuffer_Malloc));
    exports.Set("CXTUResourceUsage_SourceManager_Membuffer_MMap", Napi::Number::New(env, CXTUResourceUsage_SourceManager_Membuffer_MMap));
    exports.Set("CXTUResourceUsage_ExternalASTSource_Membuffer_Malloc", Napi::Number::New(env, CXTUResourceUsage_ExternalASTSource_Membuffer_Malloc));
    exports.Set("CXTUResourceUsage_ExternalASTSource_Membuffer_MMap", Napi::Number::New(env, CXTUResourceUsage_ExternalASTSource_Membuffer_MMap));
    exports.Set("CXTUResourceUsage_Preprocessor", Napi::Number::New(env, CXTUResourceUsage_Preprocessor));
    exports.Set("CXTUResourceUsage_PreprocessingRecord", Napi::Number::New(env, CXTUResourceUsage_PreprocessingRecord));
    exports.Set("CXTUResourceUsage_SourceManager_DataStructures", Napi::Number::New(env, CXTUResourceUsage_SourceManager_DataStructures));
    exports.Set("CXTUResourceUsage_Preprocessor_HeaderSearch", Napi::Number::New(env, CXTUResourceUsage_Preprocessor_HeaderSearch));
    exports.Set("CXTUResourceUsage_MEMORY_IN_BYTES_BEGIN", Napi::Number::New(env, CXTUResourceUsage_MEMORY_IN_BYTES_BEGIN));
    exports.Set("CXTUResourceUsage_MEMORY_IN_BYTES_END", Napi::Number::New(env, CXTUResourceUsage_MEMORY_IN_BYTES_END));
    exports.Set("CXTUResourceUsage_First", Napi::Number::New(env, CXTUResourceUsage_First));
    exports.Set("CXTUResourceUsage_Last", Napi::Number::New(env, CXTUResourceUsage_Last));
    exports.Set("CXCursor_UnexposedDecl", Napi::Number::New(env, CXCursor_UnexposedDecl));
    exports.Set("CXCursor_StructDecl", Napi::Number::New(env, CXCursor_StructDecl));
    exports.Set("CXCursor_UnionDecl", Napi::Number::New(env, CXCursor_UnionDecl));
    exports.Set("CXCursor_ClassDecl", Napi::Number::New(env, CXCursor_ClassDecl));
    exports.Set("CXCursor_EnumDecl", Napi::Number::New(env, CXCursor_EnumDecl));
    exports.Set("CXCursor_FieldDecl", Napi::Number::New(env, CXCursor_FieldDecl));
    exports.Set("CXCursor_EnumConstantDecl", Napi::Number::New(env, CXCursor_EnumConstantDecl));
    exports.Set("CXCursor_FunctionDecl", Napi::Number::New(env, CXCursor_FunctionDecl));
    exports.Set("CXCursor_VarDecl", Napi::Number::New(env, CXCursor_VarDecl));
    exports.Set("CXCursor_ParmDecl", Napi::Number::New(env, CXCursor_ParmDecl));
    exports.Set("CXCursor_ObjCInterfaceDecl", Napi::Number::New(env, CXCursor_ObjCInterfaceDecl));
    exports.Set("CXCursor_ObjCCategoryDecl", Napi::Number::New(env, CXCursor_ObjCCategoryDecl));
    exports.Set("CXCursor_ObjCProtocolDecl", Napi::Number::New(env, CXCursor_ObjCProtocolDecl));
    exports.Set("CXCursor_ObjCPropertyDecl", Napi::Number::New(env, CXCursor_ObjCPropertyDecl));
    exports.Set("CXCursor_ObjCIvarDecl", Napi::Number::New(env, CXCursor_ObjCIvarDecl));
    exports.Set("CXCursor_ObjCInstanceMethodDecl", Napi::Number::New(env, CXCursor_ObjCInstanceMethodDecl));
    exports.Set("CXCursor_ObjCClassMethodDecl", Napi::Number::New(env, CXCursor_ObjCClassMethodDecl));
    exports.Set("CXCursor_ObjCImplementationDecl", Napi::Number::New(env, CXCursor_ObjCImplementationDecl));
    exports.Set("CXCursor_ObjCCategoryImplDecl", Napi::Number::New(env, CXCursor_ObjCCategoryImplDecl));
    exports.Set("CXCursor_TypedefDecl", Napi::Number::New(env, CXCursor_TypedefDecl));
    exports.Set("CXCursor_CXXMethod", Napi::Number::New(env, CXCursor_CXXMethod));
    exports.Set("CXCursor_Namespace", Napi::Number::New(env, CXCursor_Namespace));
    exports.Set("CXCursor_LinkageSpec", Napi::Number::New(env, CXCursor_LinkageSpec));
    exports.Set("CXCursor_Constructor", Napi::Number::New(env, CXCursor_Constructor));
    exports.Set("CXCursor_Destructor", Napi::Number::New(env, CXCursor_Destructor));
    exports.Set("CXCursor_ConversionFunction", Napi::Number::New(env, CXCursor_ConversionFunction));
    exports.Set("CXCursor_TemplateTypeParameter", Napi::Number::New(env, CXCursor_TemplateTypeParameter));
    exports.Set("CXCursor_NonTypeTemplateParameter", Napi::Number::New(env, CXCursor_NonTypeTemplateParameter));
    exports.Set("CXCursor_TemplateTemplateParameter", Napi::Number::New(env, CXCursor_TemplateTemplateParameter));
    exports.Set("CXCursor_FunctionTemplate", Napi::Number::New(env, CXCursor_FunctionTemplate));
    exports.Set("CXCursor_ClassTemplate", Napi::Number::New(env, CXCursor_ClassTemplate));
    exports.Set("CXCursor_ClassTemplatePartialSpecialization", Napi::Number::New(env, CXCursor_ClassTemplatePartialSpecialization));
    exports.Set("CXCursor_NamespaceAlias", Napi::Number::New(env, CXCursor_NamespaceAlias));
    exports.Set("CXCursor_UsingDirective", Napi::Number::New(env, CXCursor_UsingDirective));
    exports.Set("CXCursor_UsingDeclaration", Napi::Number::New(env, CXCursor_UsingDeclaration));
    exports.Set("CXCursor_TypeAliasDecl", Napi::Number::New(env, CXCursor_TypeAliasDecl));
    exports.Set("CXCursor_ObjCSynthesizeDecl", Napi::Number::New(env, CXCursor_ObjCSynthesizeDecl));
    exports.Set("CXCursor_ObjCDynamicDecl", Napi::Number::New(env, CXCursor_ObjCDynamicDecl));
    exports.Set("CXCursor_CXXAccessSpecifier", Napi::Number::New(env, CXCursor_CXXAccessSpecifier));
    exports.Set("CXCursor_FirstDecl", Napi::Number::New(env, CXCursor_FirstDecl));
    exports.Set("CXCursor_LastDecl", Napi::Number::New(env, CXCursor_LastDecl));
    exports.Set("CXCursor_FirstRef", Napi::Number::New(env, CXCursor_FirstRef));
    exports.Set("CXCursor_ObjCSuperClassRef", Napi::Number::New(env, CXCursor_ObjCSuperClassRef));
    exports.Set("CXCursor_ObjCProtocolRef", Napi::Number::New(env, CXCursor_ObjCProtocolRef));
    exports.Set("CXCursor_ObjCClassRef", Napi::Number::New(env, CXCursor_ObjCClassRef));
    exports.Set("CXCursor_TypeRef", Napi::Number::New(env, CXCursor_TypeRef));
    exports.Set("CXCursor_CXXBaseSpecifier", Napi::Number::New(env, CXCursor_CXXBaseSpecifier));
    exports.Set("CXCursor_TemplateRef", Napi::Number::New(env, CXCursor_TemplateRef));
    exports.Set("CXCursor_NamespaceRef", Napi::Number::New(env, CXCursor_NamespaceRef));
    exports.Set("CXCursor_MemberRef", Napi::Number::New(env, CXCursor_MemberRef));
    exports.Set("CXCursor_LabelRef", Napi::Number::New(env, CXCursor_LabelRef));
    exports.Set("CXCursor_OverloadedDeclRef", Napi::Number::New(env, CXCursor_OverloadedDeclRef));
    exports.Set("CXCursor_VariableRef", Napi::Number::New(env, CXCursor_VariableRef));
    exports.Set("CXCursor_LastRef", Napi::Number::New(env, CXCursor_LastRef));
    exports.Set("CXCursor_FirstInvalid", Napi::Number::New(env, CXCursor_FirstInvalid));
    exports.Set("CXCursor_InvalidFile", Napi::Number::New(env, CXCursor_InvalidFile));
    exports.Set("CXCursor_NoDeclFound", Napi::Number::New(env, CXCursor_NoDeclFound));
    exports.Set("CXCursor_NotImplemented", Napi::Number::New(env, CXCursor_NotImplemented));
    exports.Set("CXCursor_InvalidCode", Napi::Number::New(env, CXCursor_InvalidCode));
    exports.Set("CXCursor_LastInvalid", Napi::Number::New(env, CXCursor_LastInvalid));
    exports.Set("CXCursor_FirstExpr", Napi::Number::New(env, CXCursor_FirstExpr));
    exports.Set("CXCursor_UnexposedExpr", Napi::Number::New(env, CXCursor_UnexposedExpr));
    exports.Set("CXCursor_DeclRefExpr", Napi::Number::New(env, CXCursor_DeclRefExpr));
    exports.Set("CXCursor_MemberRefExpr", Napi::Number::New(env, CXCursor_MemberRefExpr));
    exports.Set("CXCursor_CallExpr", Napi::Number::New(env, CXCursor_CallExpr));
    exports.Set("CXCursor_ObjCMessageExpr", Napi::Number::New(env, CXCursor_ObjCMessageExpr));
    exports.Set("CXCursor_BlockExpr", Napi::Number::New(env, CXCursor_BlockExpr));
    exports.Set("CXCursor_IntegerLiteral", Napi::Number::New(env, CXCursor_IntegerLiteral));
    exports.Set("CXCursor_FloatingLiteral", Napi::Number::New(env, CXCursor_FloatingLiteral));
    exports.Set("CXCursor_ImaginaryLiteral", Napi::Number::New(env, CXCursor_ImaginaryLiteral));
    exports.Set("CXCursor_StringLiteral", Napi::Number::New(env, CXCursor_StringLiteral));
    exports.Set("CXCursor_CharacterLiteral", Napi::Number::New(env, CXCursor_CharacterLiteral));
    exports.Set("CXCursor_ParenExpr", Napi::Number::New(env, CXCursor_ParenExpr));
    exports.Set("CXCursor_UnaryOperator", Napi::Number::New(env, CXCursor_UnaryOperator));
    exports.Set("CXCursor_ArraySubscriptExpr", Napi::Number::New(env, CXCursor_ArraySubscriptExpr));
    exports.Set("CXCursor_BinaryOperator", Napi::Number::New(env, CXCursor_BinaryOperator));
    exports.Set("CXCursor_CompoundAssignOperator", Napi::Number::New(env, CXCursor_CompoundAssignOperator));
    exports.Set("CXCursor_ConditionalOperator", Napi::Number::New(env, CXCursor_ConditionalOperator));
    exports.Set("CXCursor_CStyleCastExpr", Napi::Number::New(env, CXCursor_CStyleCastExpr));
    exports.Set("CXCursor_CompoundLiteralExpr", Napi::Number::New(env, CXCursor_CompoundLiteralExpr));
    exports.Set("CXCursor_InitListExpr", Napi::Number::New(env, CXCursor_InitListExpr));
    exports.Set("CXCursor_AddrLabelExpr", Napi::Number::New(env, CXCursor_AddrLabelExpr));
    exports.Set("CXCursor_StmtExpr", Napi::Number::New(env, CXCursor_StmtExpr));
    exports.Set("CXCursor_GenericSelectionExpr", Napi::Number::New(env, CXCursor_GenericSelectionExpr));
    exports.Set("CXCursor_GNUNullExpr", Napi::Number::New(env, CXCursor_GNUNullExpr));
    exports.Set("CXCursor_CXXStaticCastExpr", Napi::Number::New(env, CXCursor_CXXStaticCastExpr));
    exports.Set("CXCursor_CXXDynamicCastExpr", Napi::Number::New(env, CXCursor_CXXDynamicCastExpr));
    exports.Set("CXCursor_CXXReinterpretCastExpr", Napi::Number::New(env, CXCursor_CXXReinterpretCastExpr));
    exports.Set("CXCursor_CXXConstCastExpr", Napi::Number::New(env, CXCursor_CXXConstCastExpr));
    exports.Set("CXCursor_CXXFunctionalCastExpr", Napi::Number::New(env, CXCursor_CXXFunctionalCastExpr));
    exports.Set("CXCursor_CXXTypeidExpr", Napi::Number::New(env, CXCursor_CXXTypeidExpr));
    exports.Set("CXCursor_CXXBoolLiteralExpr", Napi::Number::New(env, CXCursor_CXXBoolLiteralExpr));
    exports.Set("CXCursor_CXXNullPtrLiteralExpr", Napi::Number::New(env, CXCursor_CXXNullPtrLiteralExpr));
    exports.Set("CXCursor_CXXThisExpr", Napi::Number::New(env, CXCursor_CXXThisExpr));
    exports.Set("CXCursor_CXXThrowExpr", Napi::Number::New(env, CXCursor_CXXThrowExpr));
    exports.Set("CXCursor_CXXNewExpr", Napi::Number::New(env, CXCursor_CXXNewExpr));
    exports.Set("CXCursor_CXXDeleteExpr", Napi::Number::New(env, CXCursor_CXXDeleteExpr));
    exports.Set("CXCursor_UnaryExpr", Napi::Number::New(env, CXCursor_UnaryExpr));
    exports.Set("CXCursor_ObjCStringLiteral", Napi::Number::New(env, CXCursor_ObjCStringLiteral));
    exports.Set("CXCursor_ObjCEncodeExpr", Napi::Number::New(env, CXCursor_ObjCEncodeExpr));
    exports.Set("CXCursor_ObjCSelectorExpr", Napi::Number::New(env, CXCursor_ObjCSelectorExpr));
    exports.Set("CXCursor_ObjCProtocolExpr", Napi::Number::New(env, CXCursor_ObjCProtocolExpr));
    exports.Set("CXCursor_ObjCBridgedCastExpr", Napi::Number::New(env, CXCursor_ObjCBridgedCastExpr));
    exports.Set("CXCursor_PackExpansionExpr", Napi::Number::New(env, CXCursor_PackExpansionExpr));
    exports.Set("CXCursor_SizeOfPackExpr", Napi::Number::New(env, CXCursor_SizeOfPackExpr));
    exports.Set("CXCursor_LambdaExpr", Napi::Number::New(env, CXCursor_LambdaExpr));
    exports.Set("CXCursor_ObjCBoolLiteralExpr", Napi::Number::New(env, CXCursor_ObjCBoolLiteralExpr));
    exports.Set("CXCursor_ObjCSelfExpr", Napi::Number::New(env, CXCursor_ObjCSelfExpr));
    exports.Set("CXCursor_ArraySectionExpr", Napi::Number::New(env, CXCursor_ArraySectionExpr));
    exports.Set("CXCursor_ObjCAvailabilityCheckExpr", Napi::Number::New(env, CXCursor_ObjCAvailabilityCheckExpr));
    exports.Set("CXCursor_FixedPointLiteral", Napi::Number::New(env, CXCursor_FixedPointLiteral));
    exports.Set("CXCursor_OMPArrayShapingExpr", Napi::Number::New(env, CXCursor_OMPArrayShapingExpr));
    exports.Set("CXCursor_OMPIteratorExpr", Napi::Number::New(env, CXCursor_OMPIteratorExpr));
    exports.Set("CXCursor_CXXAddrspaceCastExpr", Napi::Number::New(env, CXCursor_CXXAddrspaceCastExpr));
    exports.Set("CXCursor_ConceptSpecializationExpr", Napi::Number::New(env, CXCursor_ConceptSpecializationExpr));
    exports.Set("CXCursor_RequiresExpr", Napi::Number::New(env, CXCursor_RequiresExpr));
    exports.Set("CXCursor_CXXParenListInitExpr", Napi::Number::New(env, CXCursor_CXXParenListInitExpr));
    exports.Set("CXCursor_PackIndexingExpr", Napi::Number::New(env, CXCursor_PackIndexingExpr));
    exports.Set("CXCursor_LastExpr", Napi::Number::New(env, CXCursor_LastExpr));
    exports.Set("CXCursor_FirstStmt", Napi::Number::New(env, CXCursor_FirstStmt));
    exports.Set("CXCursor_UnexposedStmt", Napi::Number::New(env, CXCursor_UnexposedStmt));
    exports.Set("CXCursor_LabelStmt", Napi::Number::New(env, CXCursor_LabelStmt));
    exports.Set("CXCursor_CompoundStmt", Napi::Number::New(env, CXCursor_CompoundStmt));
    exports.Set("CXCursor_CaseStmt", Napi::Number::New(env, CXCursor_CaseStmt));
    exports.Set("CXCursor_DefaultStmt", Napi::Number::New(env, CXCursor_DefaultStmt));
    exports.Set("CXCursor_IfStmt", Napi::Number::New(env, CXCursor_IfStmt));
    exports.Set("CXCursor_SwitchStmt", Napi::Number::New(env, CXCursor_SwitchStmt));
    exports.Set("CXCursor_WhileStmt", Napi::Number::New(env, CXCursor_WhileStmt));
    exports.Set("CXCursor_DoStmt", Napi::Number::New(env, CXCursor_DoStmt));
    exports.Set("CXCursor_ForStmt", Napi::Number::New(env, CXCursor_ForStmt));
    exports.Set("CXCursor_GotoStmt", Napi::Number::New(env, CXCursor_GotoStmt));
    exports.Set("CXCursor_IndirectGotoStmt", Napi::Number::New(env, CXCursor_IndirectGotoStmt));
    exports.Set("CXCursor_ContinueStmt", Napi::Number::New(env, CXCursor_ContinueStmt));
    exports.Set("CXCursor_BreakStmt", Napi::Number::New(env, CXCursor_BreakStmt));
    exports.Set("CXCursor_ReturnStmt", Napi::Number::New(env, CXCursor_ReturnStmt));
    exports.Set("CXCursor_GCCAsmStmt", Napi::Number::New(env, CXCursor_GCCAsmStmt));
    exports.Set("CXCursor_AsmStmt", Napi::Number::New(env, CXCursor_AsmStmt));
    exports.Set("CXCursor_ObjCAtTryStmt", Napi::Number::New(env, CXCursor_ObjCAtTryStmt));
    exports.Set("CXCursor_ObjCAtCatchStmt", Napi::Number::New(env, CXCursor_ObjCAtCatchStmt));
    exports.Set("CXCursor_ObjCAtFinallyStmt", Napi::Number::New(env, CXCursor_ObjCAtFinallyStmt));
    exports.Set("CXCursor_ObjCAtThrowStmt", Napi::Number::New(env, CXCursor_ObjCAtThrowStmt));
    exports.Set("CXCursor_ObjCAtSynchronizedStmt", Napi::Number::New(env, CXCursor_ObjCAtSynchronizedStmt));
    exports.Set("CXCursor_ObjCAutoreleasePoolStmt", Napi::Number::New(env, CXCursor_ObjCAutoreleasePoolStmt));
    exports.Set("CXCursor_ObjCForCollectionStmt", Napi::Number::New(env, CXCursor_ObjCForCollectionStmt));
    exports.Set("CXCursor_CXXCatchStmt", Napi::Number::New(env, CXCursor_CXXCatchStmt));
    exports.Set("CXCursor_CXXTryStmt", Napi::Number::New(env, CXCursor_CXXTryStmt));
    exports.Set("CXCursor_CXXForRangeStmt", Napi::Number::New(env, CXCursor_CXXForRangeStmt));
    exports.Set("CXCursor_SEHTryStmt", Napi::Number::New(env, CXCursor_SEHTryStmt));
    exports.Set("CXCursor_SEHExceptStmt", Napi::Number::New(env, CXCursor_SEHExceptStmt));
    exports.Set("CXCursor_SEHFinallyStmt", Napi::Number::New(env, CXCursor_SEHFinallyStmt));
    exports.Set("CXCursor_MSAsmStmt", Napi::Number::New(env, CXCursor_MSAsmStmt));
    exports.Set("CXCursor_NullStmt", Napi::Number::New(env, CXCursor_NullStmt));
    exports.Set("CXCursor_DeclStmt", Napi::Number::New(env, CXCursor_DeclStmt));
    exports.Set("CXCursor_OMPParallelDirective", Napi::Number::New(env, CXCursor_OMPParallelDirective));
    exports.Set("CXCursor_OMPSimdDirective", Napi::Number::New(env, CXCursor_OMPSimdDirective));
    exports.Set("CXCursor_OMPForDirective", Napi::Number::New(env, CXCursor_OMPForDirective));
    exports.Set("CXCursor_OMPSectionsDirective", Napi::Number::New(env, CXCursor_OMPSectionsDirective));
    exports.Set("CXCursor_OMPSectionDirective", Napi::Number::New(env, CXCursor_OMPSectionDirective));
    exports.Set("CXCursor_OMPSingleDirective", Napi::Number::New(env, CXCursor_OMPSingleDirective));
    exports.Set("CXCursor_OMPParallelForDirective", Napi::Number::New(env, CXCursor_OMPParallelForDirective));
    exports.Set("CXCursor_OMPParallelSectionsDirective", Napi::Number::New(env, CXCursor_OMPParallelSectionsDirective));
    exports.Set("CXCursor_OMPTaskDirective", Napi::Number::New(env, CXCursor_OMPTaskDirective));
    exports.Set("CXCursor_OMPMasterDirective", Napi::Number::New(env, CXCursor_OMPMasterDirective));
    exports.Set("CXCursor_OMPCriticalDirective", Napi::Number::New(env, CXCursor_OMPCriticalDirective));
    exports.Set("CXCursor_OMPTaskyieldDirective", Napi::Number::New(env, CXCursor_OMPTaskyieldDirective));
    exports.Set("CXCursor_OMPBarrierDirective", Napi::Number::New(env, CXCursor_OMPBarrierDirective));
    exports.Set("CXCursor_OMPTaskwaitDirective", Napi::Number::New(env, CXCursor_OMPTaskwaitDirective));
    exports.Set("CXCursor_OMPFlushDirective", Napi::Number::New(env, CXCursor_OMPFlushDirective));
    exports.Set("CXCursor_SEHLeaveStmt", Napi::Number::New(env, CXCursor_SEHLeaveStmt));
    exports.Set("CXCursor_OMPOrderedDirective", Napi::Number::New(env, CXCursor_OMPOrderedDirective));
    exports.Set("CXCursor_OMPAtomicDirective", Napi::Number::New(env, CXCursor_OMPAtomicDirective));
    exports.Set("CXCursor_OMPForSimdDirective", Napi::Number::New(env, CXCursor_OMPForSimdDirective));
    exports.Set("CXCursor_OMPParallelForSimdDirective", Napi::Number::New(env, CXCursor_OMPParallelForSimdDirective));
    exports.Set("CXCursor_OMPTargetDirective", Napi::Number::New(env, CXCursor_OMPTargetDirective));
    exports.Set("CXCursor_OMPTeamsDirective", Napi::Number::New(env, CXCursor_OMPTeamsDirective));
    exports.Set("CXCursor_OMPTaskgroupDirective", Napi::Number::New(env, CXCursor_OMPTaskgroupDirective));
    exports.Set("CXCursor_OMPCancellationPointDirective", Napi::Number::New(env, CXCursor_OMPCancellationPointDirective));
    exports.Set("CXCursor_OMPCancelDirective", Napi::Number::New(env, CXCursor_OMPCancelDirective));
    exports.Set("CXCursor_OMPTargetDataDirective", Napi::Number::New(env, CXCursor_OMPTargetDataDirective));
    exports.Set("CXCursor_OMPTaskLoopDirective", Napi::Number::New(env, CXCursor_OMPTaskLoopDirective));
    exports.Set("CXCursor_OMPTaskLoopSimdDirective", Napi::Number::New(env, CXCursor_OMPTaskLoopSimdDirective));
    exports.Set("CXCursor_OMPDistributeDirective", Napi::Number::New(env, CXCursor_OMPDistributeDirective));
    exports.Set("CXCursor_OMPTargetEnterDataDirective", Napi::Number::New(env, CXCursor_OMPTargetEnterDataDirective));
    exports.Set("CXCursor_OMPTargetExitDataDirective", Napi::Number::New(env, CXCursor_OMPTargetExitDataDirective));
    exports.Set("CXCursor_OMPTargetParallelDirective", Napi::Number::New(env, CXCursor_OMPTargetParallelDirective));
    exports.Set("CXCursor_OMPTargetParallelForDirective", Napi::Number::New(env, CXCursor_OMPTargetParallelForDirective));
    exports.Set("CXCursor_OMPTargetUpdateDirective", Napi::Number::New(env, CXCursor_OMPTargetUpdateDirective));
    exports.Set("CXCursor_OMPDistributeParallelForDirective", Napi::Number::New(env, CXCursor_OMPDistributeParallelForDirective));
    exports.Set("CXCursor_OMPDistributeParallelForSimdDirective", Napi::Number::New(env, CXCursor_OMPDistributeParallelForSimdDirective));
    exports.Set("CXCursor_OMPDistributeSimdDirective", Napi::Number::New(env, CXCursor_OMPDistributeSimdDirective));
    exports.Set("CXCursor_OMPTargetParallelForSimdDirective", Napi::Number::New(env, CXCursor_OMPTargetParallelForSimdDirective));
    exports.Set("CXCursor_OMPTargetSimdDirective", Napi::Number::New(env, CXCursor_OMPTargetSimdDirective));
    exports.Set("CXCursor_OMPTeamsDistributeDirective", Napi::Number::New(env, CXCursor_OMPTeamsDistributeDirective));
    exports.Set("CXCursor_OMPTeamsDistributeSimdDirective", Napi::Number::New(env, CXCursor_OMPTeamsDistributeSimdDirective));
    exports.Set("CXCursor_OMPTeamsDistributeParallelForSimdDirective", Napi::Number::New(env, CXCursor_OMPTeamsDistributeParallelForSimdDirective));
    exports.Set("CXCursor_OMPTeamsDistributeParallelForDirective", Napi::Number::New(env, CXCursor_OMPTeamsDistributeParallelForDirective));
    exports.Set("CXCursor_OMPTargetTeamsDirective", Napi::Number::New(env, CXCursor_OMPTargetTeamsDirective));
    exports.Set("CXCursor_OMPTargetTeamsDistributeDirective", Napi::Number::New(env, CXCursor_OMPTargetTeamsDistributeDirective));
    exports.Set("CXCursor_OMPTargetTeamsDistributeParallelForDirective", Napi::Number::New(env, CXCursor_OMPTargetTeamsDistributeParallelForDirective));
    exports.Set("CXCursor_OMPTargetTeamsDistributeParallelForSimdDirective", Napi::Number::New(env, CXCursor_OMPTargetTeamsDistributeParallelForSimdDirective));
    exports.Set("CXCursor_OMPTargetTeamsDistributeSimdDirective", Napi::Number::New(env, CXCursor_OMPTargetTeamsDistributeSimdDirective));
    exports.Set("CXCursor_BuiltinBitCastExpr", Napi::Number::New(env, CXCursor_BuiltinBitCastExpr));
    exports.Set("CXCursor_OMPMasterTaskLoopDirective", Napi::Number::New(env, CXCursor_OMPMasterTaskLoopDirective));
    exports.Set("CXCursor_OMPParallelMasterTaskLoopDirective", Napi::Number::New(env, CXCursor_OMPParallelMasterTaskLoopDirective));
    exports.Set("CXCursor_OMPMasterTaskLoopSimdDirective", Napi::Number::New(env, CXCursor_OMPMasterTaskLoopSimdDirective));
    exports.Set("CXCursor_OMPParallelMasterTaskLoopSimdDirective", Napi::Number::New(env, CXCursor_OMPParallelMasterTaskLoopSimdDirective));
    exports.Set("CXCursor_OMPParallelMasterDirective", Napi::Number::New(env, CXCursor_OMPParallelMasterDirective));
    exports.Set("CXCursor_OMPDepobjDirective", Napi::Number::New(env, CXCursor_OMPDepobjDirective));
    exports.Set("CXCursor_OMPScanDirective", Napi::Number::New(env, CXCursor_OMPScanDirective));
    exports.Set("CXCursor_OMPTileDirective", Napi::Number::New(env, CXCursor_OMPTileDirective));
    exports.Set("CXCursor_OMPCanonicalLoop", Napi::Number::New(env, CXCursor_OMPCanonicalLoop));
    exports.Set("CXCursor_OMPInteropDirective", Napi::Number::New(env, CXCursor_OMPInteropDirective));
    exports.Set("CXCursor_OMPDispatchDirective", Napi::Number::New(env, CXCursor_OMPDispatchDirective));
    exports.Set("CXCursor_OMPMaskedDirective", Napi::Number::New(env, CXCursor_OMPMaskedDirective));
    exports.Set("CXCursor_OMPUnrollDirective", Napi::Number::New(env, CXCursor_OMPUnrollDirective));
    exports.Set("CXCursor_OMPMetaDirective", Napi::Number::New(env, CXCursor_OMPMetaDirective));
    exports.Set("CXCursor_OMPGenericLoopDirective", Napi::Number::New(env, CXCursor_OMPGenericLoopDirective));
    exports.Set("CXCursor_OMPTeamsGenericLoopDirective", Napi::Number::New(env, CXCursor_OMPTeamsGenericLoopDirective));
    exports.Set("CXCursor_OMPTargetTeamsGenericLoopDirective", Napi::Number::New(env, CXCursor_OMPTargetTeamsGenericLoopDirective));
    exports.Set("CXCursor_OMPParallelGenericLoopDirective", Napi::Number::New(env, CXCursor_OMPParallelGenericLoopDirective));
    exports.Set("CXCursor_OMPTargetParallelGenericLoopDirective", Napi::Number::New(env, CXCursor_OMPTargetParallelGenericLoopDirective));
    exports.Set("CXCursor_OMPParallelMaskedDirective", Napi::Number::New(env, CXCursor_OMPParallelMaskedDirective));
    exports.Set("CXCursor_OMPMaskedTaskLoopDirective", Napi::Number::New(env, CXCursor_OMPMaskedTaskLoopDirective));
    exports.Set("CXCursor_OMPMaskedTaskLoopSimdDirective", Napi::Number::New(env, CXCursor_OMPMaskedTaskLoopSimdDirective));
    exports.Set("CXCursor_OMPParallelMaskedTaskLoopDirective", Napi::Number::New(env, CXCursor_OMPParallelMaskedTaskLoopDirective));
    exports.Set("CXCursor_OMPParallelMaskedTaskLoopSimdDirective", Napi::Number::New(env, CXCursor_OMPParallelMaskedTaskLoopSimdDirective));
    exports.Set("CXCursor_OMPErrorDirective", Napi::Number::New(env, CXCursor_OMPErrorDirective));
    exports.Set("CXCursor_OMPScopeDirective", Napi::Number::New(env, CXCursor_OMPScopeDirective));
    exports.Set("CXCursor_OMPReverseDirective", Napi::Number::New(env, CXCursor_OMPReverseDirective));
    exports.Set("CXCursor_OMPInterchangeDirective", Napi::Number::New(env, CXCursor_OMPInterchangeDirective));
    exports.Set("CXCursor_OMPAssumeDirective", Napi::Number::New(env, CXCursor_OMPAssumeDirective));
    exports.Set("CXCursor_OpenACCComputeConstruct", Napi::Number::New(env, CXCursor_OpenACCComputeConstruct));
    exports.Set("CXCursor_OpenACCLoopConstruct", Napi::Number::New(env, CXCursor_OpenACCLoopConstruct));
    exports.Set("CXCursor_OpenACCCombinedConstruct", Napi::Number::New(env, CXCursor_OpenACCCombinedConstruct));
    exports.Set("CXCursor_OpenACCDataConstruct", Napi::Number::New(env, CXCursor_OpenACCDataConstruct));
    exports.Set("CXCursor_OpenACCEnterDataConstruct", Napi::Number::New(env, CXCursor_OpenACCEnterDataConstruct));
    exports.Set("CXCursor_OpenACCExitDataConstruct", Napi::Number::New(env, CXCursor_OpenACCExitDataConstruct));
    exports.Set("CXCursor_OpenACCHostDataConstruct", Napi::Number::New(env, CXCursor_OpenACCHostDataConstruct));
    exports.Set("CXCursor_OpenACCWaitConstruct", Napi::Number::New(env, CXCursor_OpenACCWaitConstruct));
    exports.Set("CXCursor_OpenACCInitConstruct", Napi::Number::New(env, CXCursor_OpenACCInitConstruct));
    exports.Set("CXCursor_OpenACCShutdownConstruct", Napi::Number::New(env, CXCursor_OpenACCShutdownConstruct));
    exports.Set("CXCursor_OpenACCSetConstruct", Napi::Number::New(env, CXCursor_OpenACCSetConstruct));
    exports.Set("CXCursor_OpenACCUpdateConstruct", Napi::Number::New(env, CXCursor_OpenACCUpdateConstruct));
    exports.Set("CXCursor_LastStmt", Napi::Number::New(env, CXCursor_LastStmt));
    exports.Set("CXCursor_TranslationUnit", Napi::Number::New(env, CXCursor_TranslationUnit));
    exports.Set("CXCursor_FirstAttr", Napi::Number::New(env, CXCursor_FirstAttr));
    exports.Set("CXCursor_UnexposedAttr", Napi::Number::New(env, CXCursor_UnexposedAttr));
    exports.Set("CXCursor_IBActionAttr", Napi::Number::New(env, CXCursor_IBActionAttr));
    exports.Set("CXCursor_IBOutletAttr", Napi::Number::New(env, CXCursor_IBOutletAttr));
    exports.Set("CXCursor_IBOutletCollectionAttr", Napi::Number::New(env, CXCursor_IBOutletCollectionAttr));
    exports.Set("CXCursor_CXXFinalAttr", Napi::Number::New(env, CXCursor_CXXFinalAttr));
    exports.Set("CXCursor_CXXOverrideAttr", Napi::Number::New(env, CXCursor_CXXOverrideAttr));
    exports.Set("CXCursor_AnnotateAttr", Napi::Number::New(env, CXCursor_AnnotateAttr));
    exports.Set("CXCursor_AsmLabelAttr", Napi::Number::New(env, CXCursor_AsmLabelAttr));
    exports.Set("CXCursor_PackedAttr", Napi::Number::New(env, CXCursor_PackedAttr));
    exports.Set("CXCursor_PureAttr", Napi::Number::New(env, CXCursor_PureAttr));
    exports.Set("CXCursor_ConstAttr", Napi::Number::New(env, CXCursor_ConstAttr));
    exports.Set("CXCursor_NoDuplicateAttr", Napi::Number::New(env, CXCursor_NoDuplicateAttr));
    exports.Set("CXCursor_CUDAConstantAttr", Napi::Number::New(env, CXCursor_CUDAConstantAttr));
    exports.Set("CXCursor_CUDADeviceAttr", Napi::Number::New(env, CXCursor_CUDADeviceAttr));
    exports.Set("CXCursor_CUDAGlobalAttr", Napi::Number::New(env, CXCursor_CUDAGlobalAttr));
    exports.Set("CXCursor_CUDAHostAttr", Napi::Number::New(env, CXCursor_CUDAHostAttr));
    exports.Set("CXCursor_CUDASharedAttr", Napi::Number::New(env, CXCursor_CUDASharedAttr));
    exports.Set("CXCursor_VisibilityAttr", Napi::Number::New(env, CXCursor_VisibilityAttr));
    exports.Set("CXCursor_DLLExport", Napi::Number::New(env, CXCursor_DLLExport));
    exports.Set("CXCursor_DLLImport", Napi::Number::New(env, CXCursor_DLLImport));
    exports.Set("CXCursor_NSReturnsRetained", Napi::Number::New(env, CXCursor_NSReturnsRetained));
    exports.Set("CXCursor_NSReturnsNotRetained", Napi::Number::New(env, CXCursor_NSReturnsNotRetained));
    exports.Set("CXCursor_NSReturnsAutoreleased", Napi::Number::New(env, CXCursor_NSReturnsAutoreleased));
    exports.Set("CXCursor_NSConsumesSelf", Napi::Number::New(env, CXCursor_NSConsumesSelf));
    exports.Set("CXCursor_NSConsumed", Napi::Number::New(env, CXCursor_NSConsumed));
    exports.Set("CXCursor_ObjCException", Napi::Number::New(env, CXCursor_ObjCException));
    exports.Set("CXCursor_ObjCNSObject", Napi::Number::New(env, CXCursor_ObjCNSObject));
    exports.Set("CXCursor_ObjCIndependentClass", Napi::Number::New(env, CXCursor_ObjCIndependentClass));
    exports.Set("CXCursor_ObjCPreciseLifetime", Napi::Number::New(env, CXCursor_ObjCPreciseLifetime));
    exports.Set("CXCursor_ObjCReturnsInnerPointer", Napi::Number::New(env, CXCursor_ObjCReturnsInnerPointer));
    exports.Set("CXCursor_ObjCRequiresSuper", Napi::Number::New(env, CXCursor_ObjCRequiresSuper));
    exports.Set("CXCursor_ObjCRootClass", Napi::Number::New(env, CXCursor_ObjCRootClass));
    exports.Set("CXCursor_ObjCSubclassingRestricted", Napi::Number::New(env, CXCursor_ObjCSubclassingRestricted));
    exports.Set("CXCursor_ObjCExplicitProtocolImpl", Napi::Number::New(env, CXCursor_ObjCExplicitProtocolImpl));
    exports.Set("CXCursor_ObjCDesignatedInitializer", Napi::Number::New(env, CXCursor_ObjCDesignatedInitializer));
    exports.Set("CXCursor_ObjCRuntimeVisible", Napi::Number::New(env, CXCursor_ObjCRuntimeVisible));
    exports.Set("CXCursor_ObjCBoxable", Napi::Number::New(env, CXCursor_ObjCBoxable));
    exports.Set("CXCursor_FlagEnum", Napi::Number::New(env, CXCursor_FlagEnum));
    exports.Set("CXCursor_ConvergentAttr", Napi::Number::New(env, CXCursor_ConvergentAttr));
    exports.Set("CXCursor_WarnUnusedAttr", Napi::Number::New(env, CXCursor_WarnUnusedAttr));
    exports.Set("CXCursor_WarnUnusedResultAttr", Napi::Number::New(env, CXCursor_WarnUnusedResultAttr));
    exports.Set("CXCursor_AlignedAttr", Napi::Number::New(env, CXCursor_AlignedAttr));
    exports.Set("CXCursor_LastAttr", Napi::Number::New(env, CXCursor_LastAttr));
    exports.Set("CXCursor_PreprocessingDirective", Napi::Number::New(env, CXCursor_PreprocessingDirective));
    exports.Set("CXCursor_MacroDefinition", Napi::Number::New(env, CXCursor_MacroDefinition));
    exports.Set("CXCursor_MacroExpansion", Napi::Number::New(env, CXCursor_MacroExpansion));
    exports.Set("CXCursor_MacroInstantiation", Napi::Number::New(env, CXCursor_MacroInstantiation));
    exports.Set("CXCursor_InclusionDirective", Napi::Number::New(env, CXCursor_InclusionDirective));
    exports.Set("CXCursor_FirstPreprocessing", Napi::Number::New(env, CXCursor_FirstPreprocessing));
    exports.Set("CXCursor_LastPreprocessing", Napi::Number::New(env, CXCursor_LastPreprocessing));
    exports.Set("CXCursor_ModuleImportDecl", Napi::Number::New(env, CXCursor_ModuleImportDecl));
    exports.Set("CXCursor_TypeAliasTemplateDecl", Napi::Number::New(env, CXCursor_TypeAliasTemplateDecl));
    exports.Set("CXCursor_StaticAssert", Napi::Number::New(env, CXCursor_StaticAssert));
    exports.Set("CXCursor_FriendDecl", Napi::Number::New(env, CXCursor_FriendDecl));
    exports.Set("CXCursor_ConceptDecl", Napi::Number::New(env, CXCursor_ConceptDecl));
    exports.Set("CXCursor_FirstExtraDecl", Napi::Number::New(env, CXCursor_FirstExtraDecl));
    exports.Set("CXCursor_LastExtraDecl", Napi::Number::New(env, CXCursor_LastExtraDecl));
    exports.Set("CXCursor_OverloadCandidate", Napi::Number::New(env, CXCursor_OverloadCandidate));
    exports.Set("CXLinkage_Invalid", Napi::Number::New(env, CXLinkage_Invalid));
    exports.Set("CXLinkage_NoLinkage", Napi::Number::New(env, CXLinkage_NoLinkage));
    exports.Set("CXLinkage_Internal", Napi::Number::New(env, CXLinkage_Internal));
    exports.Set("CXLinkage_UniqueExternal", Napi::Number::New(env, CXLinkage_UniqueExternal));
    exports.Set("CXLinkage_External", Napi::Number::New(env, CXLinkage_External));
    exports.Set("CXVisibility_Invalid", Napi::Number::New(env, CXVisibility_Invalid));
    exports.Set("CXVisibility_Hidden", Napi::Number::New(env, CXVisibility_Hidden));
    exports.Set("CXVisibility_Protected", Napi::Number::New(env, CXVisibility_Protected));
    exports.Set("CXVisibility_Default", Napi::Number::New(env, CXVisibility_Default));
    exports.Set("CXLanguage_Invalid", Napi::Number::New(env, CXLanguage_Invalid));
    exports.Set("CXLanguage_C", Napi::Number::New(env, CXLanguage_C));
    exports.Set("CXLanguage_ObjC", Napi::Number::New(env, CXLanguage_ObjC));
    exports.Set("CXLanguage_CPlusPlus", Napi::Number::New(env, CXLanguage_CPlusPlus));
    exports.Set("CXTLS_None", Napi::Number::New(env, CXTLS_None));
    exports.Set("CXTLS_Dynamic", Napi::Number::New(env, CXTLS_Dynamic));
    exports.Set("CXTLS_Static", Napi::Number::New(env, CXTLS_Static));
    exports.Set("CXType_Invalid", Napi::Number::New(env, CXType_Invalid));
    exports.Set("CXType_Unexposed", Napi::Number::New(env, CXType_Unexposed));
    exports.Set("CXType_Void", Napi::Number::New(env, CXType_Void));
    exports.Set("CXType_Bool", Napi::Number::New(env, CXType_Bool));
    exports.Set("CXType_Char_U", Napi::Number::New(env, CXType_Char_U));
    exports.Set("CXType_UChar", Napi::Number::New(env, CXType_UChar));
    exports.Set("CXType_Char16", Napi::Number::New(env, CXType_Char16));
    exports.Set("CXType_Char32", Napi::Number::New(env, CXType_Char32));
    exports.Set("CXType_UShort", Napi::Number::New(env, CXType_UShort));
    exports.Set("CXType_UInt", Napi::Number::New(env, CXType_UInt));
    exports.Set("CXType_ULong", Napi::Number::New(env, CXType_ULong));
    exports.Set("CXType_ULongLong", Napi::Number::New(env, CXType_ULongLong));
    exports.Set("CXType_UInt128", Napi::Number::New(env, CXType_UInt128));
    exports.Set("CXType_Char_S", Napi::Number::New(env, CXType_Char_S));
    exports.Set("CXType_SChar", Napi::Number::New(env, CXType_SChar));
    exports.Set("CXType_WChar", Napi::Number::New(env, CXType_WChar));
    exports.Set("CXType_Short", Napi::Number::New(env, CXType_Short));
    exports.Set("CXType_Int", Napi::Number::New(env, CXType_Int));
    exports.Set("CXType_Long", Napi::Number::New(env, CXType_Long));
    exports.Set("CXType_LongLong", Napi::Number::New(env, CXType_LongLong));
    exports.Set("CXType_Int128", Napi::Number::New(env, CXType_Int128));
    exports.Set("CXType_Float", Napi::Number::New(env, CXType_Float));
    exports.Set("CXType_Double", Napi::Number::New(env, CXType_Double));
    exports.Set("CXType_LongDouble", Napi::Number::New(env, CXType_LongDouble));
    exports.Set("CXType_NullPtr", Napi::Number::New(env, CXType_NullPtr));
    exports.Set("CXType_Overload", Napi::Number::New(env, CXType_Overload));
    exports.Set("CXType_Dependent", Napi::Number::New(env, CXType_Dependent));
    exports.Set("CXType_ObjCId", Napi::Number::New(env, CXType_ObjCId));
    exports.Set("CXType_ObjCClass", Napi::Number::New(env, CXType_ObjCClass));
    exports.Set("CXType_ObjCSel", Napi::Number::New(env, CXType_ObjCSel));
    exports.Set("CXType_Float128", Napi::Number::New(env, CXType_Float128));
    exports.Set("CXType_Half", Napi::Number::New(env, CXType_Half));
    exports.Set("CXType_Float16", Napi::Number::New(env, CXType_Float16));
    exports.Set("CXType_ShortAccum", Napi::Number::New(env, CXType_ShortAccum));
    exports.Set("CXType_Accum", Napi::Number::New(env, CXType_Accum));
    exports.Set("CXType_LongAccum", Napi::Number::New(env, CXType_LongAccum));
    exports.Set("CXType_UShortAccum", Napi::Number::New(env, CXType_UShortAccum));
    exports.Set("CXType_UAccum", Napi::Number::New(env, CXType_UAccum));
    exports.Set("CXType_ULongAccum", Napi::Number::New(env, CXType_ULongAccum));
    exports.Set("CXType_BFloat16", Napi::Number::New(env, CXType_BFloat16));
    exports.Set("CXType_Ibm128", Napi::Number::New(env, CXType_Ibm128));
    exports.Set("CXType_FirstBuiltin", Napi::Number::New(env, CXType_FirstBuiltin));
    exports.Set("CXType_LastBuiltin", Napi::Number::New(env, CXType_LastBuiltin));
    exports.Set("CXType_Complex", Napi::Number::New(env, CXType_Complex));
    exports.Set("CXType_Pointer", Napi::Number::New(env, CXType_Pointer));
    exports.Set("CXType_BlockPointer", Napi::Number::New(env, CXType_BlockPointer));
    exports.Set("CXType_LValueReference", Napi::Number::New(env, CXType_LValueReference));
    exports.Set("CXType_RValueReference", Napi::Number::New(env, CXType_RValueReference));
    exports.Set("CXType_Record", Napi::Number::New(env, CXType_Record));
    exports.Set("CXType_Enum", Napi::Number::New(env, CXType_Enum));
    exports.Set("CXType_Typedef", Napi::Number::New(env, CXType_Typedef));
    exports.Set("CXType_ObjCInterface", Napi::Number::New(env, CXType_ObjCInterface));
    exports.Set("CXType_ObjCObjectPointer", Napi::Number::New(env, CXType_ObjCObjectPointer));
    exports.Set("CXType_FunctionNoProto", Napi::Number::New(env, CXType_FunctionNoProto));
    exports.Set("CXType_FunctionProto", Napi::Number::New(env, CXType_FunctionProto));
    exports.Set("CXType_ConstantArray", Napi::Number::New(env, CXType_ConstantArray));
    exports.Set("CXType_Vector", Napi::Number::New(env, CXType_Vector));
    exports.Set("CXType_IncompleteArray", Napi::Number::New(env, CXType_IncompleteArray));
    exports.Set("CXType_VariableArray", Napi::Number::New(env, CXType_VariableArray));
    exports.Set("CXType_DependentSizedArray", Napi::Number::New(env, CXType_DependentSizedArray));
    exports.Set("CXType_MemberPointer", Napi::Number::New(env, CXType_MemberPointer));
    exports.Set("CXType_Auto", Napi::Number::New(env, CXType_Auto));
    exports.Set("CXType_Elaborated", Napi::Number::New(env, CXType_Elaborated));
    exports.Set("CXType_Pipe", Napi::Number::New(env, CXType_Pipe));
    exports.Set("CXType_OCLImage1dRO", Napi::Number::New(env, CXType_OCLImage1dRO));
    exports.Set("CXType_OCLImage1dArrayRO", Napi::Number::New(env, CXType_OCLImage1dArrayRO));
    exports.Set("CXType_OCLImage1dBufferRO", Napi::Number::New(env, CXType_OCLImage1dBufferRO));
    exports.Set("CXType_OCLImage2dRO", Napi::Number::New(env, CXType_OCLImage2dRO));
    exports.Set("CXType_OCLImage2dArrayRO", Napi::Number::New(env, CXType_OCLImage2dArrayRO));
    exports.Set("CXType_OCLImage2dDepthRO", Napi::Number::New(env, CXType_OCLImage2dDepthRO));
    exports.Set("CXType_OCLImage2dArrayDepthRO", Napi::Number::New(env, CXType_OCLImage2dArrayDepthRO));
    exports.Set("CXType_OCLImage2dMSAARO", Napi::Number::New(env, CXType_OCLImage2dMSAARO));
    exports.Set("CXType_OCLImage2dArrayMSAARO", Napi::Number::New(env, CXType_OCLImage2dArrayMSAARO));
    exports.Set("CXType_OCLImage2dMSAADepthRO", Napi::Number::New(env, CXType_OCLImage2dMSAADepthRO));
    exports.Set("CXType_OCLImage2dArrayMSAADepthRO", Napi::Number::New(env, CXType_OCLImage2dArrayMSAADepthRO));
    exports.Set("CXType_OCLImage3dRO", Napi::Number::New(env, CXType_OCLImage3dRO));
    exports.Set("CXType_OCLImage1dWO", Napi::Number::New(env, CXType_OCLImage1dWO));
    exports.Set("CXType_OCLImage1dArrayWO", Napi::Number::New(env, CXType_OCLImage1dArrayWO));
    exports.Set("CXType_OCLImage1dBufferWO", Napi::Number::New(env, CXType_OCLImage1dBufferWO));
    exports.Set("CXType_OCLImage2dWO", Napi::Number::New(env, CXType_OCLImage2dWO));
    exports.Set("CXType_OCLImage2dArrayWO", Napi::Number::New(env, CXType_OCLImage2dArrayWO));
    exports.Set("CXType_OCLImage2dDepthWO", Napi::Number::New(env, CXType_OCLImage2dDepthWO));
    exports.Set("CXType_OCLImage2dArrayDepthWO", Napi::Number::New(env, CXType_OCLImage2dArrayDepthWO));
    exports.Set("CXType_OCLImage2dMSAAWO", Napi::Number::New(env, CXType_OCLImage2dMSAAWO));
    exports.Set("CXType_OCLImage2dArrayMSAAWO", Napi::Number::New(env, CXType_OCLImage2dArrayMSAAWO));
    exports.Set("CXType_OCLImage2dMSAADepthWO", Napi::Number::New(env, CXType_OCLImage2dMSAADepthWO));
    exports.Set("CXType_OCLImage2dArrayMSAADepthWO", Napi::Number::New(env, CXType_OCLImage2dArrayMSAADepthWO));
    exports.Set("CXType_OCLImage3dWO", Napi::Number::New(env, CXType_OCLImage3dWO));
    exports.Set("CXType_OCLImage1dRW", Napi::Number::New(env, CXType_OCLImage1dRW));
    exports.Set("CXType_OCLImage1dArrayRW", Napi::Number::New(env, CXType_OCLImage1dArrayRW));
    exports.Set("CXType_OCLImage1dBufferRW", Napi::Number::New(env, CXType_OCLImage1dBufferRW));
    exports.Set("CXType_OCLImage2dRW", Napi::Number::New(env, CXType_OCLImage2dRW));
    exports.Set("CXType_OCLImage2dArrayRW", Napi::Number::New(env, CXType_OCLImage2dArrayRW));
    exports.Set("CXType_OCLImage2dDepthRW", Napi::Number::New(env, CXType_OCLImage2dDepthRW));
    exports.Set("CXType_OCLImage2dArrayDepthRW", Napi::Number::New(env, CXType_OCLImage2dArrayDepthRW));
    exports.Set("CXType_OCLImage2dMSAARW", Napi::Number::New(env, CXType_OCLImage2dMSAARW));
    exports.Set("CXType_OCLImage2dArrayMSAARW", Napi::Number::New(env, CXType_OCLImage2dArrayMSAARW));
    exports.Set("CXType_OCLImage2dMSAADepthRW", Napi::Number::New(env, CXType_OCLImage2dMSAADepthRW));
    exports.Set("CXType_OCLImage2dArrayMSAADepthRW", Napi::Number::New(env, CXType_OCLImage2dArrayMSAADepthRW));
    exports.Set("CXType_OCLImage3dRW", Napi::Number::New(env, CXType_OCLImage3dRW));
    exports.Set("CXType_OCLSampler", Napi::Number::New(env, CXType_OCLSampler));
    exports.Set("CXType_OCLEvent", Napi::Number::New(env, CXType_OCLEvent));
    exports.Set("CXType_OCLQueue", Napi::Number::New(env, CXType_OCLQueue));
    exports.Set("CXType_OCLReserveID", Napi::Number::New(env, CXType_OCLReserveID));
    exports.Set("CXType_ObjCObject", Napi::Number::New(env, CXType_ObjCObject));
    exports.Set("CXType_ObjCTypeParam", Napi::Number::New(env, CXType_ObjCTypeParam));
    exports.Set("CXType_Attributed", Napi::Number::New(env, CXType_Attributed));
    exports.Set("CXType_OCLIntelSubgroupAVCMcePayload", Napi::Number::New(env, CXType_OCLIntelSubgroupAVCMcePayload));
    exports.Set("CXType_OCLIntelSubgroupAVCImePayload", Napi::Number::New(env, CXType_OCLIntelSubgroupAVCImePayload));
    exports.Set("CXType_OCLIntelSubgroupAVCRefPayload", Napi::Number::New(env, CXType_OCLIntelSubgroupAVCRefPayload));
    exports.Set("CXType_OCLIntelSubgroupAVCSicPayload", Napi::Number::New(env, CXType_OCLIntelSubgroupAVCSicPayload));
    exports.Set("CXType_OCLIntelSubgroupAVCMceResult", Napi::Number::New(env, CXType_OCLIntelSubgroupAVCMceResult));
    exports.Set("CXType_OCLIntelSubgroupAVCImeResult", Napi::Number::New(env, CXType_OCLIntelSubgroupAVCImeResult));
    exports.Set("CXType_OCLIntelSubgroupAVCRefResult", Napi::Number::New(env, CXType_OCLIntelSubgroupAVCRefResult));
    exports.Set("CXType_OCLIntelSubgroupAVCSicResult", Napi::Number::New(env, CXType_OCLIntelSubgroupAVCSicResult));
    exports.Set("CXType_OCLIntelSubgroupAVCImeResultSingleReferenceStreamout", Napi::Number::New(env, CXType_OCLIntelSubgroupAVCImeResultSingleReferenceStreamout));
    exports.Set("CXType_OCLIntelSubgroupAVCImeResultDualReferenceStreamout", Napi::Number::New(env, CXType_OCLIntelSubgroupAVCImeResultDualReferenceStreamout));
    exports.Set("CXType_OCLIntelSubgroupAVCImeSingleReferenceStreamin", Napi::Number::New(env, CXType_OCLIntelSubgroupAVCImeSingleReferenceStreamin));
    exports.Set("CXType_OCLIntelSubgroupAVCImeDualReferenceStreamin", Napi::Number::New(env, CXType_OCLIntelSubgroupAVCImeDualReferenceStreamin));
    exports.Set("CXType_OCLIntelSubgroupAVCImeResultSingleRefStreamout", Napi::Number::New(env, CXType_OCLIntelSubgroupAVCImeResultSingleRefStreamout));
    exports.Set("CXType_OCLIntelSubgroupAVCImeResultDualRefStreamout", Napi::Number::New(env, CXType_OCLIntelSubgroupAVCImeResultDualRefStreamout));
    exports.Set("CXType_OCLIntelSubgroupAVCImeSingleRefStreamin", Napi::Number::New(env, CXType_OCLIntelSubgroupAVCImeSingleRefStreamin));
    exports.Set("CXType_OCLIntelSubgroupAVCImeDualRefStreamin", Napi::Number::New(env, CXType_OCLIntelSubgroupAVCImeDualRefStreamin));
    exports.Set("CXType_ExtVector", Napi::Number::New(env, CXType_ExtVector));
    exports.Set("CXType_Atomic", Napi::Number::New(env, CXType_Atomic));
    exports.Set("CXType_BTFTagAttributed", Napi::Number::New(env, CXType_BTFTagAttributed));
    exports.Set("CXType_HLSLResource", Napi::Number::New(env, CXType_HLSLResource));
    exports.Set("CXType_HLSLAttributedResource", Napi::Number::New(env, CXType_HLSLAttributedResource));
    exports.Set("CXCallingConv_Default", Napi::Number::New(env, CXCallingConv_Default));
    exports.Set("CXCallingConv_C", Napi::Number::New(env, CXCallingConv_C));
    exports.Set("CXCallingConv_X86StdCall", Napi::Number::New(env, CXCallingConv_X86StdCall));
    exports.Set("CXCallingConv_X86FastCall", Napi::Number::New(env, CXCallingConv_X86FastCall));
    exports.Set("CXCallingConv_X86ThisCall", Napi::Number::New(env, CXCallingConv_X86ThisCall));
    exports.Set("CXCallingConv_X86Pascal", Napi::Number::New(env, CXCallingConv_X86Pascal));
    exports.Set("CXCallingConv_AAPCS", Napi::Number::New(env, CXCallingConv_AAPCS));
    exports.Set("CXCallingConv_AAPCS_VFP", Napi::Number::New(env, CXCallingConv_AAPCS_VFP));
    exports.Set("CXCallingConv_X86RegCall", Napi::Number::New(env, CXCallingConv_X86RegCall));
    exports.Set("CXCallingConv_IntelOclBicc", Napi::Number::New(env, CXCallingConv_IntelOclBicc));
    exports.Set("CXCallingConv_Win64", Napi::Number::New(env, CXCallingConv_Win64));
    exports.Set("CXCallingConv_X86_64Win64", Napi::Number::New(env, CXCallingConv_X86_64Win64));
    exports.Set("CXCallingConv_X86_64SysV", Napi::Number::New(env, CXCallingConv_X86_64SysV));
    exports.Set("CXCallingConv_X86VectorCall", Napi::Number::New(env, CXCallingConv_X86VectorCall));
    exports.Set("CXCallingConv_Swift", Napi::Number::New(env, CXCallingConv_Swift));
    exports.Set("CXCallingConv_PreserveMost", Napi::Number::New(env, CXCallingConv_PreserveMost));
    exports.Set("CXCallingConv_PreserveAll", Napi::Number::New(env, CXCallingConv_PreserveAll));
    exports.Set("CXCallingConv_AArch64VectorCall", Napi::Number::New(env, CXCallingConv_AArch64VectorCall));
    exports.Set("CXCallingConv_SwiftAsync", Napi::Number::New(env, CXCallingConv_SwiftAsync));
    exports.Set("CXCallingConv_AArch64SVEPCS", Napi::Number::New(env, CXCallingConv_AArch64SVEPCS));
    exports.Set("CXCallingConv_M68kRTD", Napi::Number::New(env, CXCallingConv_M68kRTD));
    exports.Set("CXCallingConv_PreserveNone", Napi::Number::New(env, CXCallingConv_PreserveNone));
    exports.Set("CXCallingConv_RISCVVectorCall", Napi::Number::New(env, CXCallingConv_RISCVVectorCall));
    exports.Set("CXCallingConv_Invalid", Napi::Number::New(env, CXCallingConv_Invalid));
    exports.Set("CXCallingConv_Unexposed", Napi::Number::New(env, CXCallingConv_Unexposed));
    exports.Set("CXTemplateArgumentKind_Null", Napi::Number::New(env, CXTemplateArgumentKind_Null));
    exports.Set("CXTemplateArgumentKind_Type", Napi::Number::New(env, CXTemplateArgumentKind_Type));
    exports.Set("CXTemplateArgumentKind_Declaration", Napi::Number::New(env, CXTemplateArgumentKind_Declaration));
    exports.Set("CXTemplateArgumentKind_NullPtr", Napi::Number::New(env, CXTemplateArgumentKind_NullPtr));
    exports.Set("CXTemplateArgumentKind_Integral", Napi::Number::New(env, CXTemplateArgumentKind_Integral));
    exports.Set("CXTemplateArgumentKind_Template", Napi::Number::New(env, CXTemplateArgumentKind_Template));
    exports.Set("CXTemplateArgumentKind_TemplateExpansion", Napi::Number::New(env, CXTemplateArgumentKind_TemplateExpansion));
    exports.Set("CXTemplateArgumentKind_Expression", Napi::Number::New(env, CXTemplateArgumentKind_Expression));
    exports.Set("CXTemplateArgumentKind_Pack", Napi::Number::New(env, CXTemplateArgumentKind_Pack));
    exports.Set("CXTemplateArgumentKind_Invalid", Napi::Number::New(env, CXTemplateArgumentKind_Invalid));
    exports.Set("CXTypeNullability_NonNull", Napi::Number::New(env, CXTypeNullability_NonNull));
    exports.Set("CXTypeNullability_Nullable", Napi::Number::New(env, CXTypeNullability_Nullable));
    exports.Set("CXTypeNullability_Unspecified", Napi::Number::New(env, CXTypeNullability_Unspecified));
    exports.Set("CXTypeNullability_Invalid", Napi::Number::New(env, CXTypeNullability_Invalid));
    exports.Set("CXTypeNullability_NullableResult", Napi::Number::New(env, CXTypeNullability_NullableResult));
    exports.Set("CXTypeLayoutError_Invalid", Napi::Number::New(env, CXTypeLayoutError_Invalid));
    exports.Set("CXTypeLayoutError_Incomplete", Napi::Number::New(env, CXTypeLayoutError_Incomplete));
    exports.Set("CXTypeLayoutError_Dependent", Napi::Number::New(env, CXTypeLayoutError_Dependent));
    exports.Set("CXTypeLayoutError_NotConstantSize", Napi::Number::New(env, CXTypeLayoutError_NotConstantSize));
    exports.Set("CXTypeLayoutError_InvalidFieldName", Napi::Number::New(env, CXTypeLayoutError_InvalidFieldName));
    exports.Set("CXTypeLayoutError_Undeduced", Napi::Number::New(env, CXTypeLayoutError_Undeduced));
    exports.Set("CXRefQualifier_None", Napi::Number::New(env, CXRefQualifier_None));
    exports.Set("CXRefQualifier_LValue", Napi::Number::New(env, CXRefQualifier_LValue));
    exports.Set("CXRefQualifier_RValue", Napi::Number::New(env, CXRefQualifier_RValue));
    exports.Set("CX_CXXInvalidAccessSpecifier", Napi::Number::New(env, CX_CXXInvalidAccessSpecifier));
    exports.Set("CX_CXXPublic", Napi::Number::New(env, CX_CXXPublic));
    exports.Set("CX_CXXProtected", Napi::Number::New(env, CX_CXXProtected));
    exports.Set("CX_CXXPrivate", Napi::Number::New(env, CX_CXXPrivate));
    exports.Set("CX_SC_Invalid", Napi::Number::New(env, CX_SC_Invalid));
    exports.Set("CX_SC_None", Napi::Number::New(env, CX_SC_None));
    exports.Set("CX_SC_Extern", Napi::Number::New(env, CX_SC_Extern));
    exports.Set("CX_SC_Static", Napi::Number::New(env, CX_SC_Static));
    exports.Set("CX_SC_PrivateExtern", Napi::Number::New(env, CX_SC_PrivateExtern));
    exports.Set("CX_SC_OpenCLWorkGroupLocal", Napi::Number::New(env, CX_SC_OpenCLWorkGroupLocal));
    exports.Set("CX_SC_Auto", Napi::Number::New(env, CX_SC_Auto));
    exports.Set("CX_SC_Register", Napi::Number::New(env, CX_SC_Register));
    exports.Set("CX_BO_Invalid", Napi::Number::New(env, CX_BO_Invalid));
    exports.Set("CX_BO_PtrMemD", Napi::Number::New(env, CX_BO_PtrMemD));
    exports.Set("CX_BO_PtrMemI", Napi::Number::New(env, CX_BO_PtrMemI));
    exports.Set("CX_BO_Mul", Napi::Number::New(env, CX_BO_Mul));
    exports.Set("CX_BO_Div", Napi::Number::New(env, CX_BO_Div));
    exports.Set("CX_BO_Rem", Napi::Number::New(env, CX_BO_Rem));
    exports.Set("CX_BO_Add", Napi::Number::New(env, CX_BO_Add));
    exports.Set("CX_BO_Sub", Napi::Number::New(env, CX_BO_Sub));
    exports.Set("CX_BO_Shl", Napi::Number::New(env, CX_BO_Shl));
    exports.Set("CX_BO_Shr", Napi::Number::New(env, CX_BO_Shr));
    exports.Set("CX_BO_Cmp", Napi::Number::New(env, CX_BO_Cmp));
    exports.Set("CX_BO_LT", Napi::Number::New(env, CX_BO_LT));
    exports.Set("CX_BO_GT", Napi::Number::New(env, CX_BO_GT));
    exports.Set("CX_BO_LE", Napi::Number::New(env, CX_BO_LE));
    exports.Set("CX_BO_GE", Napi::Number::New(env, CX_BO_GE));
    exports.Set("CX_BO_EQ", Napi::Number::New(env, CX_BO_EQ));
    exports.Set("CX_BO_NE", Napi::Number::New(env, CX_BO_NE));
    exports.Set("CX_BO_And", Napi::Number::New(env, CX_BO_And));
    exports.Set("CX_BO_Xor", Napi::Number::New(env, CX_BO_Xor));
    exports.Set("CX_BO_Or", Napi::Number::New(env, CX_BO_Or));
    exports.Set("CX_BO_LAnd", Napi::Number::New(env, CX_BO_LAnd));
    exports.Set("CX_BO_LOr", Napi::Number::New(env, CX_BO_LOr));
    exports.Set("CX_BO_Assign", Napi::Number::New(env, CX_BO_Assign));
    exports.Set("CX_BO_MulAssign", Napi::Number::New(env, CX_BO_MulAssign));
    exports.Set("CX_BO_DivAssign", Napi::Number::New(env, CX_BO_DivAssign));
    exports.Set("CX_BO_RemAssign", Napi::Number::New(env, CX_BO_RemAssign));
    exports.Set("CX_BO_AddAssign", Napi::Number::New(env, CX_BO_AddAssign));
    exports.Set("CX_BO_SubAssign", Napi::Number::New(env, CX_BO_SubAssign));
    exports.Set("CX_BO_ShlAssign", Napi::Number::New(env, CX_BO_ShlAssign));
    exports.Set("CX_BO_ShrAssign", Napi::Number::New(env, CX_BO_ShrAssign));
    exports.Set("CX_BO_AndAssign", Napi::Number::New(env, CX_BO_AndAssign));
    exports.Set("CX_BO_XorAssign", Napi::Number::New(env, CX_BO_XorAssign));
    exports.Set("CX_BO_OrAssign", Napi::Number::New(env, CX_BO_OrAssign));
    exports.Set("CX_BO_Comma", Napi::Number::New(env, CX_BO_Comma));
    exports.Set("CX_BO_LAST", Napi::Number::New(env, CX_BO_LAST));
    exports.Set("CXChildVisit_Break", Napi::Number::New(env, CXChildVisit_Break));
    exports.Set("CXChildVisit_Continue", Napi::Number::New(env, CXChildVisit_Continue));
    exports.Set("CXChildVisit_Recurse", Napi::Number::New(env, CXChildVisit_Recurse));
    exports.Set("CXPrintingPolicy_Indentation", Napi::Number::New(env, CXPrintingPolicy_Indentation));
    exports.Set("CXPrintingPolicy_SuppressSpecifiers", Napi::Number::New(env, CXPrintingPolicy_SuppressSpecifiers));
    exports.Set("CXPrintingPolicy_SuppressTagKeyword", Napi::Number::New(env, CXPrintingPolicy_SuppressTagKeyword));
    exports.Set("CXPrintingPolicy_IncludeTagDefinition", Napi::Number::New(env, CXPrintingPolicy_IncludeTagDefinition));
    exports.Set("CXPrintingPolicy_SuppressScope", Napi::Number::New(env, CXPrintingPolicy_SuppressScope));
    exports.Set("CXPrintingPolicy_SuppressUnwrittenScope", Napi::Number::New(env, CXPrintingPolicy_SuppressUnwrittenScope));
    exports.Set("CXPrintingPolicy_SuppressInitializers", Napi::Number::New(env, CXPrintingPolicy_SuppressInitializers));
    exports.Set("CXPrintingPolicy_ConstantArraySizeAsWritten", Napi::Number::New(env, CXPrintingPolicy_ConstantArraySizeAsWritten));
    exports.Set("CXPrintingPolicy_AnonymousTagLocations", Napi::Number::New(env, CXPrintingPolicy_AnonymousTagLocations));
    exports.Set("CXPrintingPolicy_SuppressStrongLifetime", Napi::Number::New(env, CXPrintingPolicy_SuppressStrongLifetime));
    exports.Set("CXPrintingPolicy_SuppressLifetimeQualifiers", Napi::Number::New(env, CXPrintingPolicy_SuppressLifetimeQualifiers));
    exports.Set("CXPrintingPolicy_SuppressTemplateArgsInCXXConstructors", Napi::Number::New(env, CXPrintingPolicy_SuppressTemplateArgsInCXXConstructors));
    exports.Set("CXPrintingPolicy_Bool", Napi::Number::New(env, CXPrintingPolicy_Bool));
    exports.Set("CXPrintingPolicy_Restrict", Napi::Number::New(env, CXPrintingPolicy_Restrict));
    exports.Set("CXPrintingPolicy_Alignof", Napi::Number::New(env, CXPrintingPolicy_Alignof));
    exports.Set("CXPrintingPolicy_UnderscoreAlignof", Napi::Number::New(env, CXPrintingPolicy_UnderscoreAlignof));
    exports.Set("CXPrintingPolicy_UseVoidForZeroParams", Napi::Number::New(env, CXPrintingPolicy_UseVoidForZeroParams));
    exports.Set("CXPrintingPolicy_TerseOutput", Napi::Number::New(env, CXPrintingPolicy_TerseOutput));
    exports.Set("CXPrintingPolicy_PolishForDeclaration", Napi::Number::New(env, CXPrintingPolicy_PolishForDeclaration));
    exports.Set("CXPrintingPolicy_Half", Napi::Number::New(env, CXPrintingPolicy_Half));
    exports.Set("CXPrintingPolicy_MSWChar", Napi::Number::New(env, CXPrintingPolicy_MSWChar));
    exports.Set("CXPrintingPolicy_IncludeNewlines", Napi::Number::New(env, CXPrintingPolicy_IncludeNewlines));
    exports.Set("CXPrintingPolicy_MSVCFormatting", Napi::Number::New(env, CXPrintingPolicy_MSVCFormatting));
    exports.Set("CXPrintingPolicy_ConstantsAsWritten", Napi::Number::New(env, CXPrintingPolicy_ConstantsAsWritten));
    exports.Set("CXPrintingPolicy_SuppressImplicitBase", Napi::Number::New(env, CXPrintingPolicy_SuppressImplicitBase));
    exports.Set("CXPrintingPolicy_FullyQualifiedName", Napi::Number::New(env, CXPrintingPolicy_FullyQualifiedName));
    exports.Set("CXPrintingPolicy_LastProperty", Napi::Number::New(env, CXPrintingPolicy_LastProperty));
    exports.Set("CXObjCPropertyAttr_noattr", Napi::Number::New(env, CXObjCPropertyAttr_noattr));
    exports.Set("CXObjCPropertyAttr_readonly", Napi::Number::New(env, CXObjCPropertyAttr_readonly));
    exports.Set("CXObjCPropertyAttr_getter", Napi::Number::New(env, CXObjCPropertyAttr_getter));
    exports.Set("CXObjCPropertyAttr_assign", Napi::Number::New(env, CXObjCPropertyAttr_assign));
    exports.Set("CXObjCPropertyAttr_readwrite", Napi::Number::New(env, CXObjCPropertyAttr_readwrite));
    exports.Set("CXObjCPropertyAttr_retain", Napi::Number::New(env, CXObjCPropertyAttr_retain));
    exports.Set("CXObjCPropertyAttr_copy", Napi::Number::New(env, CXObjCPropertyAttr_copy));
    exports.Set("CXObjCPropertyAttr_nonatomic", Napi::Number::New(env, CXObjCPropertyAttr_nonatomic));
    exports.Set("CXObjCPropertyAttr_setter", Napi::Number::New(env, CXObjCPropertyAttr_setter));
    exports.Set("CXObjCPropertyAttr_atomic", Napi::Number::New(env, CXObjCPropertyAttr_atomic));
    exports.Set("CXObjCPropertyAttr_weak", Napi::Number::New(env, CXObjCPropertyAttr_weak));
    exports.Set("CXObjCPropertyAttr_strong", Napi::Number::New(env, CXObjCPropertyAttr_strong));
    exports.Set("CXObjCPropertyAttr_unsafe_unretained", Napi::Number::New(env, CXObjCPropertyAttr_unsafe_unretained));
    exports.Set("CXObjCPropertyAttr_class", Napi::Number::New(env, CXObjCPropertyAttr_class));
    exports.Set("CXObjCDeclQualifier_None", Napi::Number::New(env, CXObjCDeclQualifier_None));
    exports.Set("CXObjCDeclQualifier_In", Napi::Number::New(env, CXObjCDeclQualifier_In));
    exports.Set("CXObjCDeclQualifier_Inout", Napi::Number::New(env, CXObjCDeclQualifier_Inout));
    exports.Set("CXObjCDeclQualifier_Out", Napi::Number::New(env, CXObjCDeclQualifier_Out));
    exports.Set("CXObjCDeclQualifier_Bycopy", Napi::Number::New(env, CXObjCDeclQualifier_Bycopy));
    exports.Set("CXObjCDeclQualifier_Byref", Napi::Number::New(env, CXObjCDeclQualifier_Byref));
    exports.Set("CXObjCDeclQualifier_Oneway", Napi::Number::New(env, CXObjCDeclQualifier_Oneway));
    exports.Set("CXNameRange_WantQualifier", Napi::Number::New(env, CXNameRange_WantQualifier));
    exports.Set("CXNameRange_WantTemplateArgs", Napi::Number::New(env, CXNameRange_WantTemplateArgs));
    exports.Set("CXNameRange_WantSinglePiece", Napi::Number::New(env, CXNameRange_WantSinglePiece));
    exports.Set("CXToken_Punctuation", Napi::Number::New(env, CXToken_Punctuation));
    exports.Set("CXToken_Keyword", Napi::Number::New(env, CXToken_Keyword));
    exports.Set("CXToken_Identifier", Napi::Number::New(env, CXToken_Identifier));
    exports.Set("CXToken_Literal", Napi::Number::New(env, CXToken_Literal));
    exports.Set("CXToken_Comment", Napi::Number::New(env, CXToken_Comment));
    exports.Set("CXCompletionChunk_Optional", Napi::Number::New(env, CXCompletionChunk_Optional));
    exports.Set("CXCompletionChunk_TypedText", Napi::Number::New(env, CXCompletionChunk_TypedText));
    exports.Set("CXCompletionChunk_Text", Napi::Number::New(env, CXCompletionChunk_Text));
    exports.Set("CXCompletionChunk_Placeholder", Napi::Number::New(env, CXCompletionChunk_Placeholder));
    exports.Set("CXCompletionChunk_Informative", Napi::Number::New(env, CXCompletionChunk_Informative));
    exports.Set("CXCompletionChunk_CurrentParameter", Napi::Number::New(env, CXCompletionChunk_CurrentParameter));
    exports.Set("CXCompletionChunk_LeftParen", Napi::Number::New(env, CXCompletionChunk_LeftParen));
    exports.Set("CXCompletionChunk_RightParen", Napi::Number::New(env, CXCompletionChunk_RightParen));
    exports.Set("CXCompletionChunk_LeftBracket", Napi::Number::New(env, CXCompletionChunk_LeftBracket));
    exports.Set("CXCompletionChunk_RightBracket", Napi::Number::New(env, CXCompletionChunk_RightBracket));
    exports.Set("CXCompletionChunk_LeftBrace", Napi::Number::New(env, CXCompletionChunk_LeftBrace));
    exports.Set("CXCompletionChunk_RightBrace", Napi::Number::New(env, CXCompletionChunk_RightBrace));
    exports.Set("CXCompletionChunk_LeftAngle", Napi::Number::New(env, CXCompletionChunk_LeftAngle));
    exports.Set("CXCompletionChunk_RightAngle", Napi::Number::New(env, CXCompletionChunk_RightAngle));
    exports.Set("CXCompletionChunk_Comma", Napi::Number::New(env, CXCompletionChunk_Comma));
    exports.Set("CXCompletionChunk_ResultType", Napi::Number::New(env, CXCompletionChunk_ResultType));
    exports.Set("CXCompletionChunk_Colon", Napi::Number::New(env, CXCompletionChunk_Colon));
    exports.Set("CXCompletionChunk_SemiColon", Napi::Number::New(env, CXCompletionChunk_SemiColon));
    exports.Set("CXCompletionChunk_Equal", Napi::Number::New(env, CXCompletionChunk_Equal));
    exports.Set("CXCompletionChunk_HorizontalSpace", Napi::Number::New(env, CXCompletionChunk_HorizontalSpace));
    exports.Set("CXCompletionChunk_VerticalSpace", Napi::Number::New(env, CXCompletionChunk_VerticalSpace));
    exports.Set("CXCodeComplete_IncludeMacros", Napi::Number::New(env, CXCodeComplete_IncludeMacros));
    exports.Set("CXCodeComplete_IncludeCodePatterns", Napi::Number::New(env, CXCodeComplete_IncludeCodePatterns));
    exports.Set("CXCodeComplete_IncludeBriefComments", Napi::Number::New(env, CXCodeComplete_IncludeBriefComments));
    exports.Set("CXCodeComplete_SkipPreamble", Napi::Number::New(env, CXCodeComplete_SkipPreamble));
    exports.Set("CXCodeComplete_IncludeCompletionsWithFixIts", Napi::Number::New(env, CXCodeComplete_IncludeCompletionsWithFixIts));
    exports.Set("CXCompletionContext_Unexposed", Napi::Number::New(env, CXCompletionContext_Unexposed));
    exports.Set("CXCompletionContext_AnyType", Napi::Number::New(env, CXCompletionContext_AnyType));
    exports.Set("CXCompletionContext_AnyValue", Napi::Number::New(env, CXCompletionContext_AnyValue));
    exports.Set("CXCompletionContext_ObjCObjectValue", Napi::Number::New(env, CXCompletionContext_ObjCObjectValue));
    exports.Set("CXCompletionContext_ObjCSelectorValue", Napi::Number::New(env, CXCompletionContext_ObjCSelectorValue));
    exports.Set("CXCompletionContext_CXXClassTypeValue", Napi::Number::New(env, CXCompletionContext_CXXClassTypeValue));
    exports.Set("CXCompletionContext_DotMemberAccess", Napi::Number::New(env, CXCompletionContext_DotMemberAccess));
    exports.Set("CXCompletionContext_ArrowMemberAccess", Napi::Number::New(env, CXCompletionContext_ArrowMemberAccess));
    exports.Set("CXCompletionContext_ObjCPropertyAccess", Napi::Number::New(env, CXCompletionContext_ObjCPropertyAccess));
    exports.Set("CXCompletionContext_EnumTag", Napi::Number::New(env, CXCompletionContext_EnumTag));
    exports.Set("CXCompletionContext_UnionTag", Napi::Number::New(env, CXCompletionContext_UnionTag));
    exports.Set("CXCompletionContext_StructTag", Napi::Number::New(env, CXCompletionContext_StructTag));
    exports.Set("CXCompletionContext_ClassTag", Napi::Number::New(env, CXCompletionContext_ClassTag));
    exports.Set("CXCompletionContext_Namespace", Napi::Number::New(env, CXCompletionContext_Namespace));
    exports.Set("CXCompletionContext_NestedNameSpecifier", Napi::Number::New(env, CXCompletionContext_NestedNameSpecifier));
    exports.Set("CXCompletionContext_ObjCInterface", Napi::Number::New(env, CXCompletionContext_ObjCInterface));
    exports.Set("CXCompletionContext_ObjCProtocol", Napi::Number::New(env, CXCompletionContext_ObjCProtocol));
    exports.Set("CXCompletionContext_ObjCCategory", Napi::Number::New(env, CXCompletionContext_ObjCCategory));
    exports.Set("CXCompletionContext_ObjCInstanceMessage", Napi::Number::New(env, CXCompletionContext_ObjCInstanceMessage));
    exports.Set("CXCompletionContext_ObjCClassMessage", Napi::Number::New(env, CXCompletionContext_ObjCClassMessage));
    exports.Set("CXCompletionContext_ObjCSelectorName", Napi::Number::New(env, CXCompletionContext_ObjCSelectorName));
    exports.Set("CXCompletionContext_MacroName", Napi::Number::New(env, CXCompletionContext_MacroName));
    exports.Set("CXCompletionContext_NaturalLanguage", Napi::Number::New(env, CXCompletionContext_NaturalLanguage));
    exports.Set("CXCompletionContext_IncludedFile", Napi::Number::New(env, CXCompletionContext_IncludedFile));
    exports.Set("CXCompletionContext_Unknown", Napi::Number::New(env, CXCompletionContext_Unknown));
    exports.Set("CXEval_Int", Napi::Number::New(env, CXEval_Int));
    exports.Set("CXEval_Float", Napi::Number::New(env, CXEval_Float));
    exports.Set("CXEval_ObjCStrLiteral", Napi::Number::New(env, CXEval_ObjCStrLiteral));
    exports.Set("CXEval_StrLiteral", Napi::Number::New(env, CXEval_StrLiteral));
    exports.Set("CXEval_CFStr", Napi::Number::New(env, CXEval_CFStr));
    exports.Set("CXEval_Other", Napi::Number::New(env, CXEval_Other));
    exports.Set("CXEval_UnExposed", Napi::Number::New(env, CXEval_UnExposed));
    exports.Set("CXVisit_Break", Napi::Number::New(env, CXVisit_Break));
    exports.Set("CXVisit_Continue", Napi::Number::New(env, CXVisit_Continue));
    exports.Set("CXResult_Success", Napi::Number::New(env, CXResult_Success));
    exports.Set("CXResult_Invalid", Napi::Number::New(env, CXResult_Invalid));
    exports.Set("CXResult_VisitBreak", Napi::Number::New(env, CXResult_VisitBreak));
    exports.Set("CXIdxEntity_Unexposed", Napi::Number::New(env, CXIdxEntity_Unexposed));
    exports.Set("CXIdxEntity_Typedef", Napi::Number::New(env, CXIdxEntity_Typedef));
    exports.Set("CXIdxEntity_Function", Napi::Number::New(env, CXIdxEntity_Function));
    exports.Set("CXIdxEntity_Variable", Napi::Number::New(env, CXIdxEntity_Variable));
    exports.Set("CXIdxEntity_Field", Napi::Number::New(env, CXIdxEntity_Field));
    exports.Set("CXIdxEntity_EnumConstant", Napi::Number::New(env, CXIdxEntity_EnumConstant));
    exports.Set("CXIdxEntity_ObjCClass", Napi::Number::New(env, CXIdxEntity_ObjCClass));
    exports.Set("CXIdxEntity_ObjCProtocol", Napi::Number::New(env, CXIdxEntity_ObjCProtocol));
    exports.Set("CXIdxEntity_ObjCCategory", Napi::Number::New(env, CXIdxEntity_ObjCCategory));
    exports.Set("CXIdxEntity_ObjCInstanceMethod", Napi::Number::New(env, CXIdxEntity_ObjCInstanceMethod));
    exports.Set("CXIdxEntity_ObjCClassMethod", Napi::Number::New(env, CXIdxEntity_ObjCClassMethod));
    exports.Set("CXIdxEntity_ObjCProperty", Napi::Number::New(env, CXIdxEntity_ObjCProperty));
    exports.Set("CXIdxEntity_ObjCIvar", Napi::Number::New(env, CXIdxEntity_ObjCIvar));
    exports.Set("CXIdxEntity_Enum", Napi::Number::New(env, CXIdxEntity_Enum));
    exports.Set("CXIdxEntity_Struct", Napi::Number::New(env, CXIdxEntity_Struct));
    exports.Set("CXIdxEntity_Union", Napi::Number::New(env, CXIdxEntity_Union));
    exports.Set("CXIdxEntity_CXXClass", Napi::Number::New(env, CXIdxEntity_CXXClass));
    exports.Set("CXIdxEntity_CXXNamespace", Napi::Number::New(env, CXIdxEntity_CXXNamespace));
    exports.Set("CXIdxEntity_CXXNamespaceAlias", Napi::Number::New(env, CXIdxEntity_CXXNamespaceAlias));
    exports.Set("CXIdxEntity_CXXStaticVariable", Napi::Number::New(env, CXIdxEntity_CXXStaticVariable));
    exports.Set("CXIdxEntity_CXXStaticMethod", Napi::Number::New(env, CXIdxEntity_CXXStaticMethod));
    exports.Set("CXIdxEntity_CXXInstanceMethod", Napi::Number::New(env, CXIdxEntity_CXXInstanceMethod));
    exports.Set("CXIdxEntity_CXXConstructor", Napi::Number::New(env, CXIdxEntity_CXXConstructor));
    exports.Set("CXIdxEntity_CXXDestructor", Napi::Number::New(env, CXIdxEntity_CXXDestructor));
    exports.Set("CXIdxEntity_CXXConversionFunction", Napi::Number::New(env, CXIdxEntity_CXXConversionFunction));
    exports.Set("CXIdxEntity_CXXTypeAlias", Napi::Number::New(env, CXIdxEntity_CXXTypeAlias));
    exports.Set("CXIdxEntity_CXXInterface", Napi::Number::New(env, CXIdxEntity_CXXInterface));
    exports.Set("CXIdxEntity_CXXConcept", Napi::Number::New(env, CXIdxEntity_CXXConcept));
    exports.Set("CXIdxEntityLang_None", Napi::Number::New(env, CXIdxEntityLang_None));
    exports.Set("CXIdxEntityLang_C", Napi::Number::New(env, CXIdxEntityLang_C));
    exports.Set("CXIdxEntityLang_ObjC", Napi::Number::New(env, CXIdxEntityLang_ObjC));
    exports.Set("CXIdxEntityLang_CXX", Napi::Number::New(env, CXIdxEntityLang_CXX));
    exports.Set("CXIdxEntityLang_Swift", Napi::Number::New(env, CXIdxEntityLang_Swift));
    exports.Set("CXIdxEntity_NonTemplate", Napi::Number::New(env, CXIdxEntity_NonTemplate));
    exports.Set("CXIdxEntity_Template", Napi::Number::New(env, CXIdxEntity_Template));
    exports.Set("CXIdxEntity_TemplatePartialSpecialization", Napi::Number::New(env, CXIdxEntity_TemplatePartialSpecialization));
    exports.Set("CXIdxEntity_TemplateSpecialization", Napi::Number::New(env, CXIdxEntity_TemplateSpecialization));
    exports.Set("CXIdxAttr_Unexposed", Napi::Number::New(env, CXIdxAttr_Unexposed));
    exports.Set("CXIdxAttr_IBAction", Napi::Number::New(env, CXIdxAttr_IBAction));
    exports.Set("CXIdxAttr_IBOutlet", Napi::Number::New(env, CXIdxAttr_IBOutlet));
    exports.Set("CXIdxAttr_IBOutletCollection", Napi::Number::New(env, CXIdxAttr_IBOutletCollection));
    exports.Set("CXIdxDeclFlag_Skipped", Napi::Number::New(env, CXIdxDeclFlag_Skipped));
    exports.Set("CXIdxObjCContainer_ForwardRef", Napi::Number::New(env, CXIdxObjCContainer_ForwardRef));
    exports.Set("CXIdxObjCContainer_Interface", Napi::Number::New(env, CXIdxObjCContainer_Interface));
    exports.Set("CXIdxObjCContainer_Implementation", Napi::Number::New(env, CXIdxObjCContainer_Implementation));
    exports.Set("CXIdxEntityRef_Direct", Napi::Number::New(env, CXIdxEntityRef_Direct));
    exports.Set("CXIdxEntityRef_Implicit", Napi::Number::New(env, CXIdxEntityRef_Implicit));
    exports.Set("CXSymbolRole_None", Napi::Number::New(env, CXSymbolRole_None));
    exports.Set("CXSymbolRole_Declaration", Napi::Number::New(env, CXSymbolRole_Declaration));
    exports.Set("CXSymbolRole_Definition", Napi::Number::New(env, CXSymbolRole_Definition));
    exports.Set("CXSymbolRole_Reference", Napi::Number::New(env, CXSymbolRole_Reference));
    exports.Set("CXSymbolRole_Read", Napi::Number::New(env, CXSymbolRole_Read));
    exports.Set("CXSymbolRole_Write", Napi::Number::New(env, CXSymbolRole_Write));
    exports.Set("CXSymbolRole_Call", Napi::Number::New(env, CXSymbolRole_Call));
    exports.Set("CXSymbolRole_Dynamic", Napi::Number::New(env, CXSymbolRole_Dynamic));
    exports.Set("CXSymbolRole_AddressOf", Napi::Number::New(env, CXSymbolRole_AddressOf));
    exports.Set("CXSymbolRole_Implicit", Napi::Number::New(env, CXSymbolRole_Implicit));
    exports.Set("CXIndexOpt_None", Napi::Number::New(env, CXIndexOpt_None));
    exports.Set("CXIndexOpt_SuppressRedundantRefs", Napi::Number::New(env, CXIndexOpt_SuppressRedundantRefs));
    exports.Set("CXIndexOpt_IndexFunctionLocalSymbols", Napi::Number::New(env, CXIndexOpt_IndexFunctionLocalSymbols));
    exports.Set("CXIndexOpt_IndexImplicitTemplateInstantiations", Napi::Number::New(env, CXIndexOpt_IndexImplicitTemplateInstantiations));
    exports.Set("CXIndexOpt_SuppressWarnings", Napi::Number::New(env, CXIndexOpt_SuppressWarnings));
    exports.Set("CXIndexOpt_SkipParsedBodiesInSession", Napi::Number::New(env, CXIndexOpt_SkipParsedBodiesInSession));
    exports.Set("CXBinaryOperator_Invalid", Napi::Number::New(env, CXBinaryOperator_Invalid));
    exports.Set("CXBinaryOperator_PtrMemD", Napi::Number::New(env, CXBinaryOperator_PtrMemD));
    exports.Set("CXBinaryOperator_PtrMemI", Napi::Number::New(env, CXBinaryOperator_PtrMemI));
    exports.Set("CXBinaryOperator_Mul", Napi::Number::New(env, CXBinaryOperator_Mul));
    exports.Set("CXBinaryOperator_Div", Napi::Number::New(env, CXBinaryOperator_Div));
    exports.Set("CXBinaryOperator_Rem", Napi::Number::New(env, CXBinaryOperator_Rem));
    exports.Set("CXBinaryOperator_Add", Napi::Number::New(env, CXBinaryOperator_Add));
    exports.Set("CXBinaryOperator_Sub", Napi::Number::New(env, CXBinaryOperator_Sub));
    exports.Set("CXBinaryOperator_Shl", Napi::Number::New(env, CXBinaryOperator_Shl));
    exports.Set("CXBinaryOperator_Shr", Napi::Number::New(env, CXBinaryOperator_Shr));
    exports.Set("CXBinaryOperator_Cmp", Napi::Number::New(env, CXBinaryOperator_Cmp));
    exports.Set("CXBinaryOperator_LT", Napi::Number::New(env, CXBinaryOperator_LT));
    exports.Set("CXBinaryOperator_GT", Napi::Number::New(env, CXBinaryOperator_GT));
    exports.Set("CXBinaryOperator_LE", Napi::Number::New(env, CXBinaryOperator_LE));
    exports.Set("CXBinaryOperator_GE", Napi::Number::New(env, CXBinaryOperator_GE));
    exports.Set("CXBinaryOperator_EQ", Napi::Number::New(env, CXBinaryOperator_EQ));
    exports.Set("CXBinaryOperator_NE", Napi::Number::New(env, CXBinaryOperator_NE));
    exports.Set("CXBinaryOperator_And", Napi::Number::New(env, CXBinaryOperator_And));
    exports.Set("CXBinaryOperator_Xor", Napi::Number::New(env, CXBinaryOperator_Xor));
    exports.Set("CXBinaryOperator_Or", Napi::Number::New(env, CXBinaryOperator_Or));
    exports.Set("CXBinaryOperator_LAnd", Napi::Number::New(env, CXBinaryOperator_LAnd));
    exports.Set("CXBinaryOperator_LOr", Napi::Number::New(env, CXBinaryOperator_LOr));
    exports.Set("CXBinaryOperator_Assign", Napi::Number::New(env, CXBinaryOperator_Assign));
    exports.Set("CXBinaryOperator_MulAssign", Napi::Number::New(env, CXBinaryOperator_MulAssign));
    exports.Set("CXBinaryOperator_DivAssign", Napi::Number::New(env, CXBinaryOperator_DivAssign));
    exports.Set("CXBinaryOperator_RemAssign", Napi::Number::New(env, CXBinaryOperator_RemAssign));
    exports.Set("CXBinaryOperator_AddAssign", Napi::Number::New(env, CXBinaryOperator_AddAssign));
    exports.Set("CXBinaryOperator_SubAssign", Napi::Number::New(env, CXBinaryOperator_SubAssign));
    exports.Set("CXBinaryOperator_ShlAssign", Napi::Number::New(env, CXBinaryOperator_ShlAssign));
    exports.Set("CXBinaryOperator_ShrAssign", Napi::Number::New(env, CXBinaryOperator_ShrAssign));
    exports.Set("CXBinaryOperator_AndAssign", Napi::Number::New(env, CXBinaryOperator_AndAssign));
    exports.Set("CXBinaryOperator_XorAssign", Napi::Number::New(env, CXBinaryOperator_XorAssign));
    exports.Set("CXBinaryOperator_OrAssign", Napi::Number::New(env, CXBinaryOperator_OrAssign));
    exports.Set("CXBinaryOperator_Comma", Napi::Number::New(env, CXBinaryOperator_Comma));
    exports.Set("CXUnaryOperator_Invalid", Napi::Number::New(env, CXUnaryOperator_Invalid));
    exports.Set("CXUnaryOperator_PostInc", Napi::Number::New(env, CXUnaryOperator_PostInc));
    exports.Set("CXUnaryOperator_PostDec", Napi::Number::New(env, CXUnaryOperator_PostDec));
    exports.Set("CXUnaryOperator_PreInc", Napi::Number::New(env, CXUnaryOperator_PreInc));
    exports.Set("CXUnaryOperator_PreDec", Napi::Number::New(env, CXUnaryOperator_PreDec));
    exports.Set("CXUnaryOperator_AddrOf", Napi::Number::New(env, CXUnaryOperator_AddrOf));
    exports.Set("CXUnaryOperator_Deref", Napi::Number::New(env, CXUnaryOperator_Deref));
    exports.Set("CXUnaryOperator_Plus", Napi::Number::New(env, CXUnaryOperator_Plus));
    exports.Set("CXUnaryOperator_Minus", Napi::Number::New(env, CXUnaryOperator_Minus));
    exports.Set("CXUnaryOperator_Not", Napi::Number::New(env, CXUnaryOperator_Not));
    exports.Set("CXUnaryOperator_LNot", Napi::Number::New(env, CXUnaryOperator_LNot));
    exports.Set("CXUnaryOperator_Real", Napi::Number::New(env, CXUnaryOperator_Real));
    exports.Set("CXUnaryOperator_Imag", Napi::Number::New(env, CXUnaryOperator_Imag));
    exports.Set("CXUnaryOperator_Extension", Napi::Number::New(env, CXUnaryOperator_Extension));
    exports.Set("CXUnaryOperator_Coawait", Napi::Number::New(env, CXUnaryOperator_Coawait));
    exports.Set("create_CXString", Napi::Function::New(env, Create_CXString));
    exports.Set("get_CXString_field", Napi::Function::New(env, Get_CXString_Field));
    exports.Set("create_CXStringSet", Napi::Function::New(env, Create_CXStringSet));
    exports.Set("get_CXStringSet_field", Napi::Function::New(env, Get_CXStringSet_Field));
    exports.Set("create_CXFileUniqueID", Napi::Function::New(env, Create_CXFileUniqueID));
    exports.Set("get_CXFileUniqueID_field", Napi::Function::New(env, Get_CXFileUniqueID_Field));
    exports.Set("create_CXSourceLocation", Napi::Function::New(env, Create_CXSourceLocation));
    exports.Set("get_CXSourceLocation_field", Napi::Function::New(env, Get_CXSourceLocation_Field));
    exports.Set("create_CXSourceRange", Napi::Function::New(env, Create_CXSourceRange));
    exports.Set("get_CXSourceRange_field", Napi::Function::New(env, Get_CXSourceRange_Field));
    exports.Set("create_CXSourceRangeList", Napi::Function::New(env, Create_CXSourceRangeList));
    exports.Set("get_CXSourceRangeList_field", Napi::Function::New(env, Get_CXSourceRangeList_Field));
    exports.Set("create_CXUnsavedFile", Napi::Function::New(env, Create_CXUnsavedFile));
    exports.Set("get_CXUnsavedFile_field", Napi::Function::New(env, Get_CXUnsavedFile_Field));
    exports.Set("create_CXVersion", Napi::Function::New(env, Create_CXVersion));
    exports.Set("get_CXVersion_field", Napi::Function::New(env, Get_CXVersion_Field));
    exports.Set("create_CXIndexOptions", Napi::Function::New(env, Create_CXIndexOptions));
    exports.Set("get_CXIndexOptions_field", Napi::Function::New(env, Get_CXIndexOptions_Field));
    exports.Set("create_CXTUResourceUsageEntry", Napi::Function::New(env, Create_CXTUResourceUsageEntry));
    exports.Set("get_CXTUResourceUsageEntry_field", Napi::Function::New(env, Get_CXTUResourceUsageEntry_Field));
    exports.Set("create_CXTUResourceUsage", Napi::Function::New(env, Create_CXTUResourceUsage));
    exports.Set("get_CXTUResourceUsage_field", Napi::Function::New(env, Get_CXTUResourceUsage_Field));
    exports.Set("create_CXCursor", Napi::Function::New(env, Create_CXCursor));
    exports.Set("get_CXCursor_field", Napi::Function::New(env, Get_CXCursor_Field));
    exports.Set("create_CXPlatformAvailability", Napi::Function::New(env, Create_CXPlatformAvailability));
    exports.Set("get_CXPlatformAvailability_field", Napi::Function::New(env, Get_CXPlatformAvailability_Field));
    exports.Set("create_CXType", Napi::Function::New(env, Create_CXType));
    exports.Set("get_CXType_field", Napi::Function::New(env, Get_CXType_Field));
    exports.Set("create_CXToken", Napi::Function::New(env, Create_CXToken));
    exports.Set("get_CXToken_field", Napi::Function::New(env, Get_CXToken_Field));
    exports.Set("create_CXCompletionResult", Napi::Function::New(env, Create_CXCompletionResult));
    exports.Set("get_CXCompletionResult_field", Napi::Function::New(env, Get_CXCompletionResult_Field));
    exports.Set("create_CXCodeCompleteResults", Napi::Function::New(env, Create_CXCodeCompleteResults));
    exports.Set("get_CXCodeCompleteResults_field", Napi::Function::New(env, Get_CXCodeCompleteResults_Field));
    exports.Set("create_CXCursorAndRangeVisitor", Napi::Function::New(env, Create_CXCursorAndRangeVisitor));
    exports.Set("get_CXCursorAndRangeVisitor_field", Napi::Function::New(env, Get_CXCursorAndRangeVisitor_Field));
    exports.Set("create_CXIdxLoc", Napi::Function::New(env, Create_CXIdxLoc));
    exports.Set("get_CXIdxLoc_field", Napi::Function::New(env, Get_CXIdxLoc_Field));
    exports.Set("create_CXIdxIncludedFileInfo", Napi::Function::New(env, Create_CXIdxIncludedFileInfo));
    exports.Set("get_CXIdxIncludedFileInfo_field", Napi::Function::New(env, Get_CXIdxIncludedFileInfo_Field));
    exports.Set("create_CXIdxImportedASTFileInfo", Napi::Function::New(env, Create_CXIdxImportedASTFileInfo));
    exports.Set("get_CXIdxImportedASTFileInfo_field", Napi::Function::New(env, Get_CXIdxImportedASTFileInfo_Field));
    exports.Set("create_CXIdxAttrInfo", Napi::Function::New(env, Create_CXIdxAttrInfo));
    exports.Set("get_CXIdxAttrInfo_field", Napi::Function::New(env, Get_CXIdxAttrInfo_Field));
    exports.Set("create_CXIdxEntityInfo", Napi::Function::New(env, Create_CXIdxEntityInfo));
    exports.Set("get_CXIdxEntityInfo_field", Napi::Function::New(env, Get_CXIdxEntityInfo_Field));
    exports.Set("create_CXIdxContainerInfo", Napi::Function::New(env, Create_CXIdxContainerInfo));
    exports.Set("get_CXIdxContainerInfo_field", Napi::Function::New(env, Get_CXIdxContainerInfo_Field));
    exports.Set("create_CXIdxIBOutletCollectionAttrInfo", Napi::Function::New(env, Create_CXIdxIBOutletCollectionAttrInfo));
    exports.Set("get_CXIdxIBOutletCollectionAttrInfo_field", Napi::Function::New(env, Get_CXIdxIBOutletCollectionAttrInfo_Field));
    exports.Set("create_CXIdxDeclInfo", Napi::Function::New(env, Create_CXIdxDeclInfo));
    exports.Set("get_CXIdxDeclInfo_field", Napi::Function::New(env, Get_CXIdxDeclInfo_Field));
    exports.Set("create_CXIdxObjCContainerDeclInfo", Napi::Function::New(env, Create_CXIdxObjCContainerDeclInfo));
    exports.Set("get_CXIdxObjCContainerDeclInfo_field", Napi::Function::New(env, Get_CXIdxObjCContainerDeclInfo_Field));
    exports.Set("create_CXIdxBaseClassInfo", Napi::Function::New(env, Create_CXIdxBaseClassInfo));
    exports.Set("get_CXIdxBaseClassInfo_field", Napi::Function::New(env, Get_CXIdxBaseClassInfo_Field));
    exports.Set("create_CXIdxObjCProtocolRefInfo", Napi::Function::New(env, Create_CXIdxObjCProtocolRefInfo));
    exports.Set("get_CXIdxObjCProtocolRefInfo_field", Napi::Function::New(env, Get_CXIdxObjCProtocolRefInfo_Field));
    exports.Set("create_CXIdxObjCProtocolRefListInfo", Napi::Function::New(env, Create_CXIdxObjCProtocolRefListInfo));
    exports.Set("get_CXIdxObjCProtocolRefListInfo_field", Napi::Function::New(env, Get_CXIdxObjCProtocolRefListInfo_Field));
    exports.Set("create_CXIdxObjCInterfaceDeclInfo", Napi::Function::New(env, Create_CXIdxObjCInterfaceDeclInfo));
    exports.Set("get_CXIdxObjCInterfaceDeclInfo_field", Napi::Function::New(env, Get_CXIdxObjCInterfaceDeclInfo_Field));
    exports.Set("create_CXIdxObjCCategoryDeclInfo", Napi::Function::New(env, Create_CXIdxObjCCategoryDeclInfo));
    exports.Set("get_CXIdxObjCCategoryDeclInfo_field", Napi::Function::New(env, Get_CXIdxObjCCategoryDeclInfo_Field));
    exports.Set("create_CXIdxObjCPropertyDeclInfo", Napi::Function::New(env, Create_CXIdxObjCPropertyDeclInfo));
    exports.Set("get_CXIdxObjCPropertyDeclInfo_field", Napi::Function::New(env, Get_CXIdxObjCPropertyDeclInfo_Field));
    exports.Set("create_CXIdxCXXClassDeclInfo", Napi::Function::New(env, Create_CXIdxCXXClassDeclInfo));
    exports.Set("get_CXIdxCXXClassDeclInfo_field", Napi::Function::New(env, Get_CXIdxCXXClassDeclInfo_Field));
    exports.Set("create_CXIdxEntityRefInfo", Napi::Function::New(env, Create_CXIdxEntityRefInfo));
    exports.Set("get_CXIdxEntityRefInfo_field", Napi::Function::New(env, Get_CXIdxEntityRefInfo_Field));
    exports.Set("create_IndexerCallbacks", Napi::Function::New(env, Create_IndexerCallbacks));
    exports.Set("get_IndexerCallbacks_field", Napi::Function::New(env, Get_IndexerCallbacks_Field));
    exports.Set("clang_getCString", Napi::Function::New(env, clang_getCString_wrapper));
    exports.Set("clang_disposeString", Napi::Function::New(env, clang_disposeString_wrapper));
    exports.Set("clang_disposeStringSet", Napi::Function::New(env, clang_disposeStringSet_wrapper));
    exports.Set("clang_getBuildSessionTimestamp", Napi::Function::New(env, clang_getBuildSessionTimestamp_wrapper));
    exports.Set("clang_VirtualFileOverlay_create", Napi::Function::New(env, clang_VirtualFileOverlay_create_wrapper));
    exports.Set("clang_VirtualFileOverlay_addFileMapping", Napi::Function::New(env, clang_VirtualFileOverlay_addFileMapping_wrapper));
    exports.Set("clang_VirtualFileOverlay_setCaseSensitivity", Napi::Function::New(env, clang_VirtualFileOverlay_setCaseSensitivity_wrapper));
    exports.Set("clang_VirtualFileOverlay_writeToBuffer", Napi::Function::New(env, clang_VirtualFileOverlay_writeToBuffer_wrapper));
    exports.Set("clang_free", Napi::Function::New(env, clang_free_wrapper));
    exports.Set("clang_VirtualFileOverlay_dispose", Napi::Function::New(env, clang_VirtualFileOverlay_dispose_wrapper));
    exports.Set("clang_ModuleMapDescriptor_create", Napi::Function::New(env, clang_ModuleMapDescriptor_create_wrapper));
    exports.Set("clang_ModuleMapDescriptor_setFrameworkModuleName", Napi::Function::New(env, clang_ModuleMapDescriptor_setFrameworkModuleName_wrapper));
    exports.Set("clang_ModuleMapDescriptor_setUmbrellaHeader", Napi::Function::New(env, clang_ModuleMapDescriptor_setUmbrellaHeader_wrapper));
    exports.Set("clang_ModuleMapDescriptor_writeToBuffer", Napi::Function::New(env, clang_ModuleMapDescriptor_writeToBuffer_wrapper));
    exports.Set("clang_ModuleMapDescriptor_dispose", Napi::Function::New(env, clang_ModuleMapDescriptor_dispose_wrapper));
    exports.Set("clang_getFileName", Napi::Function::New(env, clang_getFileName_wrapper));
    exports.Set("clang_getFileTime", Napi::Function::New(env, clang_getFileTime_wrapper));
    exports.Set("clang_getFileUniqueID", Napi::Function::New(env, clang_getFileUniqueID_wrapper));
    exports.Set("clang_File_isEqual", Napi::Function::New(env, clang_File_isEqual_wrapper));
    exports.Set("clang_File_tryGetRealPathName", Napi::Function::New(env, clang_File_tryGetRealPathName_wrapper));
    exports.Set("clang_getNullLocation", Napi::Function::New(env, clang_getNullLocation_wrapper));
    exports.Set("clang_equalLocations", Napi::Function::New(env, clang_equalLocations_wrapper));
    exports.Set("clang_isBeforeInTranslationUnit", Napi::Function::New(env, clang_isBeforeInTranslationUnit_wrapper));
    exports.Set("clang_Location_isInSystemHeader", Napi::Function::New(env, clang_Location_isInSystemHeader_wrapper));
    exports.Set("clang_Location_isFromMainFile", Napi::Function::New(env, clang_Location_isFromMainFile_wrapper));
    exports.Set("clang_getNullRange", Napi::Function::New(env, clang_getNullRange_wrapper));
    exports.Set("clang_getRange", Napi::Function::New(env, clang_getRange_wrapper));
    exports.Set("clang_equalRanges", Napi::Function::New(env, clang_equalRanges_wrapper));
    exports.Set("clang_Range_isNull", Napi::Function::New(env, clang_Range_isNull_wrapper));
    exports.Set("clang_getExpansionLocation", Napi::Function::New(env, clang_getExpansionLocation_wrapper));
    exports.Set("clang_getPresumedLocation", Napi::Function::New(env, clang_getPresumedLocation_wrapper));
    exports.Set("clang_getInstantiationLocation", Napi::Function::New(env, clang_getInstantiationLocation_wrapper));
    exports.Set("clang_getSpellingLocation", Napi::Function::New(env, clang_getSpellingLocation_wrapper));
    exports.Set("clang_getFileLocation", Napi::Function::New(env, clang_getFileLocation_wrapper));
    exports.Set("clang_getRangeStart", Napi::Function::New(env, clang_getRangeStart_wrapper));
    exports.Set("clang_getRangeEnd", Napi::Function::New(env, clang_getRangeEnd_wrapper));
    exports.Set("clang_disposeSourceRangeList", Napi::Function::New(env, clang_disposeSourceRangeList_wrapper));
    exports.Set("clang_getNumDiagnosticsInSet", Napi::Function::New(env, clang_getNumDiagnosticsInSet_wrapper));
    exports.Set("clang_getDiagnosticInSet", Napi::Function::New(env, clang_getDiagnosticInSet_wrapper));
    exports.Set("clang_loadDiagnostics", Napi::Function::New(env, clang_loadDiagnostics_wrapper));
    exports.Set("clang_disposeDiagnosticSet", Napi::Function::New(env, clang_disposeDiagnosticSet_wrapper));
    exports.Set("clang_getChildDiagnostics", Napi::Function::New(env, clang_getChildDiagnostics_wrapper));
    exports.Set("clang_disposeDiagnostic", Napi::Function::New(env, clang_disposeDiagnostic_wrapper));
    exports.Set("clang_formatDiagnostic", Napi::Function::New(env, clang_formatDiagnostic_wrapper));
    exports.Set("clang_defaultDiagnosticDisplayOptions", Napi::Function::New(env, clang_defaultDiagnosticDisplayOptions_wrapper));
    exports.Set("clang_getDiagnosticSeverity", Napi::Function::New(env, clang_getDiagnosticSeverity_wrapper));
    exports.Set("clang_getDiagnosticLocation", Napi::Function::New(env, clang_getDiagnosticLocation_wrapper));
    exports.Set("clang_getDiagnosticSpelling", Napi::Function::New(env, clang_getDiagnosticSpelling_wrapper));
    exports.Set("clang_getDiagnosticOption", Napi::Function::New(env, clang_getDiagnosticOption_wrapper));
    exports.Set("clang_getDiagnosticCategory", Napi::Function::New(env, clang_getDiagnosticCategory_wrapper));
    exports.Set("clang_getDiagnosticCategoryName", Napi::Function::New(env, clang_getDiagnosticCategoryName_wrapper));
    exports.Set("clang_getDiagnosticCategoryText", Napi::Function::New(env, clang_getDiagnosticCategoryText_wrapper));
    exports.Set("clang_getDiagnosticNumRanges", Napi::Function::New(env, clang_getDiagnosticNumRanges_wrapper));
    exports.Set("clang_getDiagnosticRange", Napi::Function::New(env, clang_getDiagnosticRange_wrapper));
    exports.Set("clang_getDiagnosticNumFixIts", Napi::Function::New(env, clang_getDiagnosticNumFixIts_wrapper));
    exports.Set("clang_getDiagnosticFixIt", Napi::Function::New(env, clang_getDiagnosticFixIt_wrapper));
    exports.Set("clang_createIndex", Napi::Function::New(env, clang_createIndex_wrapper));
    exports.Set("clang_disposeIndex", Napi::Function::New(env, clang_disposeIndex_wrapper));
    exports.Set("clang_createIndexWithOptions", Napi::Function::New(env, clang_createIndexWithOptions_wrapper));
    exports.Set("clang_CXIndex_setGlobalOptions", Napi::Function::New(env, clang_CXIndex_setGlobalOptions_wrapper));
    exports.Set("clang_CXIndex_getGlobalOptions", Napi::Function::New(env, clang_CXIndex_getGlobalOptions_wrapper));
    exports.Set("clang_CXIndex_setInvocationEmissionPathOption", Napi::Function::New(env, clang_CXIndex_setInvocationEmissionPathOption_wrapper));
    exports.Set("clang_isFileMultipleIncludeGuarded", Napi::Function::New(env, clang_isFileMultipleIncludeGuarded_wrapper));
    exports.Set("clang_getFile", Napi::Function::New(env, clang_getFile_wrapper));
    exports.Set("clang_getFileContents", Napi::Function::New(env, clang_getFileContents_wrapper));
    exports.Set("clang_getLocation", Napi::Function::New(env, clang_getLocation_wrapper));
    exports.Set("clang_getLocationForOffset", Napi::Function::New(env, clang_getLocationForOffset_wrapper));
    exports.Set("clang_getSkippedRanges", Napi::Function::New(env, clang_getSkippedRanges_wrapper));
    exports.Set("clang_getAllSkippedRanges", Napi::Function::New(env, clang_getAllSkippedRanges_wrapper));
    exports.Set("clang_getNumDiagnostics", Napi::Function::New(env, clang_getNumDiagnostics_wrapper));
    exports.Set("clang_getDiagnostic", Napi::Function::New(env, clang_getDiagnostic_wrapper));
    exports.Set("clang_getDiagnosticSetFromTU", Napi::Function::New(env, clang_getDiagnosticSetFromTU_wrapper));
    exports.Set("clang_getTranslationUnitSpelling", Napi::Function::New(env, clang_getTranslationUnitSpelling_wrapper));
    exports.Set("clang_createTranslationUnitFromSourceFile", Napi::Function::New(env, clang_createTranslationUnitFromSourceFile_wrapper));
    exports.Set("clang_createTranslationUnit", Napi::Function::New(env, clang_createTranslationUnit_wrapper));
    exports.Set("clang_createTranslationUnit2", Napi::Function::New(env, clang_createTranslationUnit2_wrapper));
    exports.Set("clang_defaultEditingTranslationUnitOptions", Napi::Function::New(env, clang_defaultEditingTranslationUnitOptions_wrapper));
    exports.Set("clang_parseTranslationUnit", Napi::Function::New(env, clang_parseTranslationUnit_wrapper));
    exports.Set("clang_parseTranslationUnit2", Napi::Function::New(env, clang_parseTranslationUnit2_wrapper));
    exports.Set("clang_parseTranslationUnit2FullArgv", Napi::Function::New(env, clang_parseTranslationUnit2FullArgv_wrapper));
    exports.Set("clang_defaultSaveOptions", Napi::Function::New(env, clang_defaultSaveOptions_wrapper));
    exports.Set("clang_saveTranslationUnit", Napi::Function::New(env, clang_saveTranslationUnit_wrapper));
    exports.Set("clang_suspendTranslationUnit", Napi::Function::New(env, clang_suspendTranslationUnit_wrapper));
    exports.Set("clang_disposeTranslationUnit", Napi::Function::New(env, clang_disposeTranslationUnit_wrapper));
    exports.Set("clang_defaultReparseOptions", Napi::Function::New(env, clang_defaultReparseOptions_wrapper));
    exports.Set("clang_reparseTranslationUnit", Napi::Function::New(env, clang_reparseTranslationUnit_wrapper));
    exports.Set("clang_getTUResourceUsageName", Napi::Function::New(env, clang_getTUResourceUsageName_wrapper));
    exports.Set("clang_getCXTUResourceUsage", Napi::Function::New(env, clang_getCXTUResourceUsage_wrapper));
    exports.Set("clang_disposeCXTUResourceUsage", Napi::Function::New(env, clang_disposeCXTUResourceUsage_wrapper));
    exports.Set("clang_getTranslationUnitTargetInfo", Napi::Function::New(env, clang_getTranslationUnitTargetInfo_wrapper));
    exports.Set("clang_TargetInfo_dispose", Napi::Function::New(env, clang_TargetInfo_dispose_wrapper));
    exports.Set("clang_TargetInfo_getTriple", Napi::Function::New(env, clang_TargetInfo_getTriple_wrapper));
    exports.Set("clang_TargetInfo_getPointerWidth", Napi::Function::New(env, clang_TargetInfo_getPointerWidth_wrapper));
    exports.Set("clang_getNullCursor", Napi::Function::New(env, clang_getNullCursor_wrapper));
    exports.Set("clang_getTranslationUnitCursor", Napi::Function::New(env, clang_getTranslationUnitCursor_wrapper));
    exports.Set("clang_equalCursors", Napi::Function::New(env, clang_equalCursors_wrapper));
    exports.Set("clang_Cursor_isNull", Napi::Function::New(env, clang_Cursor_isNull_wrapper));
    exports.Set("clang_hashCursor", Napi::Function::New(env, clang_hashCursor_wrapper));
    exports.Set("clang_getCursorKind", Napi::Function::New(env, clang_getCursorKind_wrapper));
    exports.Set("clang_isDeclaration", Napi::Function::New(env, clang_isDeclaration_wrapper));
    exports.Set("clang_isInvalidDeclaration", Napi::Function::New(env, clang_isInvalidDeclaration_wrapper));
    exports.Set("clang_isReference", Napi::Function::New(env, clang_isReference_wrapper));
    exports.Set("clang_isExpression", Napi::Function::New(env, clang_isExpression_wrapper));
    exports.Set("clang_isStatement", Napi::Function::New(env, clang_isStatement_wrapper));
    exports.Set("clang_isAttribute", Napi::Function::New(env, clang_isAttribute_wrapper));
    exports.Set("clang_Cursor_hasAttrs", Napi::Function::New(env, clang_Cursor_hasAttrs_wrapper));
    exports.Set("clang_isInvalid", Napi::Function::New(env, clang_isInvalid_wrapper));
    exports.Set("clang_isTranslationUnit", Napi::Function::New(env, clang_isTranslationUnit_wrapper));
    exports.Set("clang_isPreprocessing", Napi::Function::New(env, clang_isPreprocessing_wrapper));
    exports.Set("clang_isUnexposed", Napi::Function::New(env, clang_isUnexposed_wrapper));
    exports.Set("clang_getCursorLinkage", Napi::Function::New(env, clang_getCursorLinkage_wrapper));
    exports.Set("clang_getCursorVisibility", Napi::Function::New(env, clang_getCursorVisibility_wrapper));
    exports.Set("clang_getCursorAvailability", Napi::Function::New(env, clang_getCursorAvailability_wrapper));
    exports.Set("clang_getCursorPlatformAvailability", Napi::Function::New(env, clang_getCursorPlatformAvailability_wrapper));
    exports.Set("clang_disposeCXPlatformAvailability", Napi::Function::New(env, clang_disposeCXPlatformAvailability_wrapper));
    exports.Set("clang_Cursor_getVarDeclInitializer", Napi::Function::New(env, clang_Cursor_getVarDeclInitializer_wrapper));
    exports.Set("clang_Cursor_hasVarDeclGlobalStorage", Napi::Function::New(env, clang_Cursor_hasVarDeclGlobalStorage_wrapper));
    exports.Set("clang_Cursor_hasVarDeclExternalStorage", Napi::Function::New(env, clang_Cursor_hasVarDeclExternalStorage_wrapper));
    exports.Set("clang_getCursorLanguage", Napi::Function::New(env, clang_getCursorLanguage_wrapper));
    exports.Set("clang_getCursorTLSKind", Napi::Function::New(env, clang_getCursorTLSKind_wrapper));
    exports.Set("clang_Cursor_getTranslationUnit", Napi::Function::New(env, clang_Cursor_getTranslationUnit_wrapper));
    exports.Set("clang_createCXCursorSet", Napi::Function::New(env, clang_createCXCursorSet_wrapper));
    exports.Set("clang_disposeCXCursorSet", Napi::Function::New(env, clang_disposeCXCursorSet_wrapper));
    exports.Set("clang_CXCursorSet_contains", Napi::Function::New(env, clang_CXCursorSet_contains_wrapper));
    exports.Set("clang_CXCursorSet_insert", Napi::Function::New(env, clang_CXCursorSet_insert_wrapper));
    exports.Set("clang_getCursorSemanticParent", Napi::Function::New(env, clang_getCursorSemanticParent_wrapper));
    exports.Set("clang_getCursorLexicalParent", Napi::Function::New(env, clang_getCursorLexicalParent_wrapper));
    exports.Set("clang_getOverriddenCursors", Napi::Function::New(env, clang_getOverriddenCursors_wrapper));
    exports.Set("clang_disposeOverriddenCursors", Napi::Function::New(env, clang_disposeOverriddenCursors_wrapper));
    exports.Set("clang_getIncludedFile", Napi::Function::New(env, clang_getIncludedFile_wrapper));
    exports.Set("clang_getCursor", Napi::Function::New(env, clang_getCursor_wrapper));
    exports.Set("clang_getCursorLocation", Napi::Function::New(env, clang_getCursorLocation_wrapper));
    exports.Set("clang_getCursorExtent", Napi::Function::New(env, clang_getCursorExtent_wrapper));
    exports.Set("clang_getCursorType", Napi::Function::New(env, clang_getCursorType_wrapper));
    exports.Set("clang_getTypeSpelling", Napi::Function::New(env, clang_getTypeSpelling_wrapper));
    exports.Set("clang_getTypedefDeclUnderlyingType", Napi::Function::New(env, clang_getTypedefDeclUnderlyingType_wrapper));
    exports.Set("clang_getEnumDeclIntegerType", Napi::Function::New(env, clang_getEnumDeclIntegerType_wrapper));
    exports.Set("clang_getEnumConstantDeclValue", Napi::Function::New(env, clang_getEnumConstantDeclValue_wrapper));
    exports.Set("clang_getEnumConstantDeclUnsignedValue", Napi::Function::New(env, clang_getEnumConstantDeclUnsignedValue_wrapper));
    exports.Set("clang_Cursor_isBitField", Napi::Function::New(env, clang_Cursor_isBitField_wrapper));
    exports.Set("clang_getFieldDeclBitWidth", Napi::Function::New(env, clang_getFieldDeclBitWidth_wrapper));
    exports.Set("clang_Cursor_getNumArguments", Napi::Function::New(env, clang_Cursor_getNumArguments_wrapper));
    exports.Set("clang_Cursor_getArgument", Napi::Function::New(env, clang_Cursor_getArgument_wrapper));
    exports.Set("clang_Cursor_getNumTemplateArguments", Napi::Function::New(env, clang_Cursor_getNumTemplateArguments_wrapper));
    exports.Set("clang_Cursor_getTemplateArgumentKind", Napi::Function::New(env, clang_Cursor_getTemplateArgumentKind_wrapper));
    exports.Set("clang_Cursor_getTemplateArgumentType", Napi::Function::New(env, clang_Cursor_getTemplateArgumentType_wrapper));
    exports.Set("clang_Cursor_getTemplateArgumentValue", Napi::Function::New(env, clang_Cursor_getTemplateArgumentValue_wrapper));
    exports.Set("clang_Cursor_getTemplateArgumentUnsignedValue", Napi::Function::New(env, clang_Cursor_getTemplateArgumentUnsignedValue_wrapper));
    exports.Set("clang_equalTypes", Napi::Function::New(env, clang_equalTypes_wrapper));
    exports.Set("clang_getCanonicalType", Napi::Function::New(env, clang_getCanonicalType_wrapper));
    exports.Set("clang_isConstQualifiedType", Napi::Function::New(env, clang_isConstQualifiedType_wrapper));
    exports.Set("clang_Cursor_isMacroFunctionLike", Napi::Function::New(env, clang_Cursor_isMacroFunctionLike_wrapper));
    exports.Set("clang_Cursor_isMacroBuiltin", Napi::Function::New(env, clang_Cursor_isMacroBuiltin_wrapper));
    exports.Set("clang_Cursor_isFunctionInlined", Napi::Function::New(env, clang_Cursor_isFunctionInlined_wrapper));
    exports.Set("clang_isVolatileQualifiedType", Napi::Function::New(env, clang_isVolatileQualifiedType_wrapper));
    exports.Set("clang_isRestrictQualifiedType", Napi::Function::New(env, clang_isRestrictQualifiedType_wrapper));
    exports.Set("clang_getAddressSpace", Napi::Function::New(env, clang_getAddressSpace_wrapper));
    exports.Set("clang_getTypedefName", Napi::Function::New(env, clang_getTypedefName_wrapper));
    exports.Set("clang_getPointeeType", Napi::Function::New(env, clang_getPointeeType_wrapper));
    exports.Set("clang_getUnqualifiedType", Napi::Function::New(env, clang_getUnqualifiedType_wrapper));
    exports.Set("clang_getNonReferenceType", Napi::Function::New(env, clang_getNonReferenceType_wrapper));
    exports.Set("clang_getTypeDeclaration", Napi::Function::New(env, clang_getTypeDeclaration_wrapper));
    exports.Set("clang_getDeclObjCTypeEncoding", Napi::Function::New(env, clang_getDeclObjCTypeEncoding_wrapper));
    exports.Set("clang_Type_getObjCEncoding", Napi::Function::New(env, clang_Type_getObjCEncoding_wrapper));
    exports.Set("clang_getTypeKindSpelling", Napi::Function::New(env, clang_getTypeKindSpelling_wrapper));
    exports.Set("clang_getFunctionTypeCallingConv", Napi::Function::New(env, clang_getFunctionTypeCallingConv_wrapper));
    exports.Set("clang_getResultType", Napi::Function::New(env, clang_getResultType_wrapper));
    exports.Set("clang_getExceptionSpecificationType", Napi::Function::New(env, clang_getExceptionSpecificationType_wrapper));
    exports.Set("clang_getNumArgTypes", Napi::Function::New(env, clang_getNumArgTypes_wrapper));
    exports.Set("clang_getArgType", Napi::Function::New(env, clang_getArgType_wrapper));
    exports.Set("clang_Type_getObjCObjectBaseType", Napi::Function::New(env, clang_Type_getObjCObjectBaseType_wrapper));
    exports.Set("clang_Type_getNumObjCProtocolRefs", Napi::Function::New(env, clang_Type_getNumObjCProtocolRefs_wrapper));
    exports.Set("clang_Type_getObjCProtocolDecl", Napi::Function::New(env, clang_Type_getObjCProtocolDecl_wrapper));
    exports.Set("clang_Type_getNumObjCTypeArgs", Napi::Function::New(env, clang_Type_getNumObjCTypeArgs_wrapper));
    exports.Set("clang_Type_getObjCTypeArg", Napi::Function::New(env, clang_Type_getObjCTypeArg_wrapper));
    exports.Set("clang_isFunctionTypeVariadic", Napi::Function::New(env, clang_isFunctionTypeVariadic_wrapper));
    exports.Set("clang_getCursorResultType", Napi::Function::New(env, clang_getCursorResultType_wrapper));
    exports.Set("clang_getCursorExceptionSpecificationType", Napi::Function::New(env, clang_getCursorExceptionSpecificationType_wrapper));
    exports.Set("clang_isPODType", Napi::Function::New(env, clang_isPODType_wrapper));
    exports.Set("clang_getElementType", Napi::Function::New(env, clang_getElementType_wrapper));
    exports.Set("clang_getNumElements", Napi::Function::New(env, clang_getNumElements_wrapper));
    exports.Set("clang_getArrayElementType", Napi::Function::New(env, clang_getArrayElementType_wrapper));
    exports.Set("clang_getArraySize", Napi::Function::New(env, clang_getArraySize_wrapper));
    exports.Set("clang_Type_getNamedType", Napi::Function::New(env, clang_Type_getNamedType_wrapper));
    exports.Set("clang_Type_isTransparentTagTypedef", Napi::Function::New(env, clang_Type_isTransparentTagTypedef_wrapper));
    exports.Set("clang_Type_getNullability", Napi::Function::New(env, clang_Type_getNullability_wrapper));
    exports.Set("clang_Type_getAlignOf", Napi::Function::New(env, clang_Type_getAlignOf_wrapper));
    exports.Set("clang_Type_getClassType", Napi::Function::New(env, clang_Type_getClassType_wrapper));
    exports.Set("clang_Type_getSizeOf", Napi::Function::New(env, clang_Type_getSizeOf_wrapper));
    exports.Set("clang_Type_getOffsetOf", Napi::Function::New(env, clang_Type_getOffsetOf_wrapper));
    exports.Set("clang_Type_getModifiedType", Napi::Function::New(env, clang_Type_getModifiedType_wrapper));
    exports.Set("clang_Type_getValueType", Napi::Function::New(env, clang_Type_getValueType_wrapper));
    exports.Set("clang_Cursor_getOffsetOfField", Napi::Function::New(env, clang_Cursor_getOffsetOfField_wrapper));
    exports.Set("clang_Cursor_isAnonymous", Napi::Function::New(env, clang_Cursor_isAnonymous_wrapper));
    exports.Set("clang_Cursor_isAnonymousRecordDecl", Napi::Function::New(env, clang_Cursor_isAnonymousRecordDecl_wrapper));
    exports.Set("clang_Cursor_isInlineNamespace", Napi::Function::New(env, clang_Cursor_isInlineNamespace_wrapper));
    exports.Set("clang_Type_getNumTemplateArguments", Napi::Function::New(env, clang_Type_getNumTemplateArguments_wrapper));
    exports.Set("clang_Type_getTemplateArgumentAsType", Napi::Function::New(env, clang_Type_getTemplateArgumentAsType_wrapper));
    exports.Set("clang_Type_getCXXRefQualifier", Napi::Function::New(env, clang_Type_getCXXRefQualifier_wrapper));
    exports.Set("clang_isVirtualBase", Napi::Function::New(env, clang_isVirtualBase_wrapper));
    exports.Set("clang_getOffsetOfBase", Napi::Function::New(env, clang_getOffsetOfBase_wrapper));
    exports.Set("clang_getCXXAccessSpecifier", Napi::Function::New(env, clang_getCXXAccessSpecifier_wrapper));
    exports.Set("clang_Cursor_getBinaryOpcode", Napi::Function::New(env, clang_Cursor_getBinaryOpcode_wrapper));
    exports.Set("clang_Cursor_getBinaryOpcodeStr", Napi::Function::New(env, clang_Cursor_getBinaryOpcodeStr_wrapper));
    exports.Set("clang_Cursor_getStorageClass", Napi::Function::New(env, clang_Cursor_getStorageClass_wrapper));
    exports.Set("clang_getNumOverloadedDecls", Napi::Function::New(env, clang_getNumOverloadedDecls_wrapper));
    exports.Set("clang_getOverloadedDecl", Napi::Function::New(env, clang_getOverloadedDecl_wrapper));
    exports.Set("clang_getIBOutletCollectionType", Napi::Function::New(env, clang_getIBOutletCollectionType_wrapper));
    exports.Set("clang_visitChildren", Napi::Function::New(env, clang_visitChildren_wrapper));
    exports.Set("clang_visitChildrenWithBlock", Napi::Function::New(env, clang_visitChildrenWithBlock_wrapper));
    exports.Set("clang_getCursorUSR", Napi::Function::New(env, clang_getCursorUSR_wrapper));
    exports.Set("clang_constructUSR_ObjCClass", Napi::Function::New(env, clang_constructUSR_ObjCClass_wrapper));
    exports.Set("clang_constructUSR_ObjCCategory", Napi::Function::New(env, clang_constructUSR_ObjCCategory_wrapper));
    exports.Set("clang_constructUSR_ObjCProtocol", Napi::Function::New(env, clang_constructUSR_ObjCProtocol_wrapper));
    exports.Set("clang_constructUSR_ObjCIvar", Napi::Function::New(env, clang_constructUSR_ObjCIvar_wrapper));
    exports.Set("clang_constructUSR_ObjCMethod", Napi::Function::New(env, clang_constructUSR_ObjCMethod_wrapper));
    exports.Set("clang_constructUSR_ObjCProperty", Napi::Function::New(env, clang_constructUSR_ObjCProperty_wrapper));
    exports.Set("clang_getCursorSpelling", Napi::Function::New(env, clang_getCursorSpelling_wrapper));
    exports.Set("clang_Cursor_getSpellingNameRange", Napi::Function::New(env, clang_Cursor_getSpellingNameRange_wrapper));
    exports.Set("clang_PrintingPolicy_getProperty", Napi::Function::New(env, clang_PrintingPolicy_getProperty_wrapper));
    exports.Set("clang_PrintingPolicy_setProperty", Napi::Function::New(env, clang_PrintingPolicy_setProperty_wrapper));
    exports.Set("clang_getCursorPrintingPolicy", Napi::Function::New(env, clang_getCursorPrintingPolicy_wrapper));
    exports.Set("clang_PrintingPolicy_dispose", Napi::Function::New(env, clang_PrintingPolicy_dispose_wrapper));
    exports.Set("clang_getCursorPrettyPrinted", Napi::Function::New(env, clang_getCursorPrettyPrinted_wrapper));
    exports.Set("clang_getTypePrettyPrinted", Napi::Function::New(env, clang_getTypePrettyPrinted_wrapper));
    exports.Set("clang_getCursorDisplayName", Napi::Function::New(env, clang_getCursorDisplayName_wrapper));
    exports.Set("clang_getCursorReferenced", Napi::Function::New(env, clang_getCursorReferenced_wrapper));
    exports.Set("clang_getCursorDefinition", Napi::Function::New(env, clang_getCursorDefinition_wrapper));
    exports.Set("clang_isCursorDefinition", Napi::Function::New(env, clang_isCursorDefinition_wrapper));
    exports.Set("clang_getCanonicalCursor", Napi::Function::New(env, clang_getCanonicalCursor_wrapper));
    exports.Set("clang_Cursor_getObjCSelectorIndex", Napi::Function::New(env, clang_Cursor_getObjCSelectorIndex_wrapper));
    exports.Set("clang_Cursor_isDynamicCall", Napi::Function::New(env, clang_Cursor_isDynamicCall_wrapper));
    exports.Set("clang_Cursor_getReceiverType", Napi::Function::New(env, clang_Cursor_getReceiverType_wrapper));
    exports.Set("clang_Cursor_getObjCPropertyAttributes", Napi::Function::New(env, clang_Cursor_getObjCPropertyAttributes_wrapper));
    exports.Set("clang_Cursor_getObjCPropertyGetterName", Napi::Function::New(env, clang_Cursor_getObjCPropertyGetterName_wrapper));
    exports.Set("clang_Cursor_getObjCPropertySetterName", Napi::Function::New(env, clang_Cursor_getObjCPropertySetterName_wrapper));
    exports.Set("clang_Cursor_getObjCDeclQualifiers", Napi::Function::New(env, clang_Cursor_getObjCDeclQualifiers_wrapper));
    exports.Set("clang_Cursor_isObjCOptional", Napi::Function::New(env, clang_Cursor_isObjCOptional_wrapper));
    exports.Set("clang_Cursor_isVariadic", Napi::Function::New(env, clang_Cursor_isVariadic_wrapper));
    exports.Set("clang_Cursor_isExternalSymbol", Napi::Function::New(env, clang_Cursor_isExternalSymbol_wrapper));
    exports.Set("clang_Cursor_getCommentRange", Napi::Function::New(env, clang_Cursor_getCommentRange_wrapper));
    exports.Set("clang_Cursor_getRawCommentText", Napi::Function::New(env, clang_Cursor_getRawCommentText_wrapper));
    exports.Set("clang_Cursor_getBriefCommentText", Napi::Function::New(env, clang_Cursor_getBriefCommentText_wrapper));
    exports.Set("clang_Cursor_getMangling", Napi::Function::New(env, clang_Cursor_getMangling_wrapper));
    exports.Set("clang_Cursor_getCXXManglings", Napi::Function::New(env, clang_Cursor_getCXXManglings_wrapper));
    exports.Set("clang_Cursor_getObjCManglings", Napi::Function::New(env, clang_Cursor_getObjCManglings_wrapper));
    exports.Set("clang_Cursor_getModule", Napi::Function::New(env, clang_Cursor_getModule_wrapper));
    exports.Set("clang_getModuleForFile", Napi::Function::New(env, clang_getModuleForFile_wrapper));
    exports.Set("clang_Module_getASTFile", Napi::Function::New(env, clang_Module_getASTFile_wrapper));
    exports.Set("clang_Module_getParent", Napi::Function::New(env, clang_Module_getParent_wrapper));
    exports.Set("clang_Module_getName", Napi::Function::New(env, clang_Module_getName_wrapper));
    exports.Set("clang_Module_getFullName", Napi::Function::New(env, clang_Module_getFullName_wrapper));
    exports.Set("clang_Module_isSystem", Napi::Function::New(env, clang_Module_isSystem_wrapper));
    exports.Set("clang_Module_getNumTopLevelHeaders", Napi::Function::New(env, clang_Module_getNumTopLevelHeaders_wrapper));
    exports.Set("clang_Module_getTopLevelHeader", Napi::Function::New(env, clang_Module_getTopLevelHeader_wrapper));
    exports.Set("clang_CXXConstructor_isConvertingConstructor", Napi::Function::New(env, clang_CXXConstructor_isConvertingConstructor_wrapper));
    exports.Set("clang_CXXConstructor_isCopyConstructor", Napi::Function::New(env, clang_CXXConstructor_isCopyConstructor_wrapper));
    exports.Set("clang_CXXConstructor_isDefaultConstructor", Napi::Function::New(env, clang_CXXConstructor_isDefaultConstructor_wrapper));
    exports.Set("clang_CXXConstructor_isMoveConstructor", Napi::Function::New(env, clang_CXXConstructor_isMoveConstructor_wrapper));
    exports.Set("clang_CXXField_isMutable", Napi::Function::New(env, clang_CXXField_isMutable_wrapper));
    exports.Set("clang_CXXMethod_isDefaulted", Napi::Function::New(env, clang_CXXMethod_isDefaulted_wrapper));
    exports.Set("clang_CXXMethod_isDeleted", Napi::Function::New(env, clang_CXXMethod_isDeleted_wrapper));
    exports.Set("clang_CXXMethod_isPureVirtual", Napi::Function::New(env, clang_CXXMethod_isPureVirtual_wrapper));
    exports.Set("clang_CXXMethod_isStatic", Napi::Function::New(env, clang_CXXMethod_isStatic_wrapper));
    exports.Set("clang_CXXMethod_isVirtual", Napi::Function::New(env, clang_CXXMethod_isVirtual_wrapper));
    exports.Set("clang_CXXMethod_isCopyAssignmentOperator", Napi::Function::New(env, clang_CXXMethod_isCopyAssignmentOperator_wrapper));
    exports.Set("clang_CXXMethod_isMoveAssignmentOperator", Napi::Function::New(env, clang_CXXMethod_isMoveAssignmentOperator_wrapper));
    exports.Set("clang_CXXMethod_isExplicit", Napi::Function::New(env, clang_CXXMethod_isExplicit_wrapper));
    exports.Set("clang_CXXRecord_isAbstract", Napi::Function::New(env, clang_CXXRecord_isAbstract_wrapper));
    exports.Set("clang_EnumDecl_isScoped", Napi::Function::New(env, clang_EnumDecl_isScoped_wrapper));
    exports.Set("clang_CXXMethod_isConst", Napi::Function::New(env, clang_CXXMethod_isConst_wrapper));
    exports.Set("clang_getTemplateCursorKind", Napi::Function::New(env, clang_getTemplateCursorKind_wrapper));
    exports.Set("clang_getSpecializedCursorTemplate", Napi::Function::New(env, clang_getSpecializedCursorTemplate_wrapper));
    exports.Set("clang_getCursorReferenceNameRange", Napi::Function::New(env, clang_getCursorReferenceNameRange_wrapper));
    exports.Set("clang_getToken", Napi::Function::New(env, clang_getToken_wrapper));
    exports.Set("clang_getTokenKind", Napi::Function::New(env, clang_getTokenKind_wrapper));
    exports.Set("clang_getTokenSpelling", Napi::Function::New(env, clang_getTokenSpelling_wrapper));
    exports.Set("clang_getTokenLocation", Napi::Function::New(env, clang_getTokenLocation_wrapper));
    exports.Set("clang_getTokenExtent", Napi::Function::New(env, clang_getTokenExtent_wrapper));
    exports.Set("clang_tokenize", Napi::Function::New(env, clang_tokenize_wrapper));
    exports.Set("clang_annotateTokens", Napi::Function::New(env, clang_annotateTokens_wrapper));
    exports.Set("clang_disposeTokens", Napi::Function::New(env, clang_disposeTokens_wrapper));
    exports.Set("clang_getCursorKindSpelling", Napi::Function::New(env, clang_getCursorKindSpelling_wrapper));
    exports.Set("clang_getDefinitionSpellingAndExtent", Napi::Function::New(env, clang_getDefinitionSpellingAndExtent_wrapper));
    exports.Set("clang_enableStackTraces", Napi::Function::New(env, clang_enableStackTraces_wrapper));
    exports.Set("clang_executeOnThread", Napi::Function::New(env, clang_executeOnThread_wrapper));
    exports.Set("clang_getCompletionChunkKind", Napi::Function::New(env, clang_getCompletionChunkKind_wrapper));
    exports.Set("clang_getCompletionChunkText", Napi::Function::New(env, clang_getCompletionChunkText_wrapper));
    exports.Set("clang_getCompletionChunkCompletionString", Napi::Function::New(env, clang_getCompletionChunkCompletionString_wrapper));
    exports.Set("clang_getNumCompletionChunks", Napi::Function::New(env, clang_getNumCompletionChunks_wrapper));
    exports.Set("clang_getCompletionPriority", Napi::Function::New(env, clang_getCompletionPriority_wrapper));
    exports.Set("clang_getCompletionAvailability", Napi::Function::New(env, clang_getCompletionAvailability_wrapper));
    exports.Set("clang_getCompletionNumAnnotations", Napi::Function::New(env, clang_getCompletionNumAnnotations_wrapper));
    exports.Set("clang_getCompletionAnnotation", Napi::Function::New(env, clang_getCompletionAnnotation_wrapper));
    exports.Set("clang_getCompletionParent", Napi::Function::New(env, clang_getCompletionParent_wrapper));
    exports.Set("clang_getCompletionBriefComment", Napi::Function::New(env, clang_getCompletionBriefComment_wrapper));
    exports.Set("clang_getCursorCompletionString", Napi::Function::New(env, clang_getCursorCompletionString_wrapper));
    exports.Set("clang_getCompletionNumFixIts", Napi::Function::New(env, clang_getCompletionNumFixIts_wrapper));
    exports.Set("clang_getCompletionFixIt", Napi::Function::New(env, clang_getCompletionFixIt_wrapper));
    exports.Set("clang_defaultCodeCompleteOptions", Napi::Function::New(env, clang_defaultCodeCompleteOptions_wrapper));
    exports.Set("clang_codeCompleteAt", Napi::Function::New(env, clang_codeCompleteAt_wrapper));
    exports.Set("clang_sortCodeCompletionResults", Napi::Function::New(env, clang_sortCodeCompletionResults_wrapper));
    exports.Set("clang_disposeCodeCompleteResults", Napi::Function::New(env, clang_disposeCodeCompleteResults_wrapper));
    exports.Set("clang_codeCompleteGetNumDiagnostics", Napi::Function::New(env, clang_codeCompleteGetNumDiagnostics_wrapper));
    exports.Set("clang_codeCompleteGetDiagnostic", Napi::Function::New(env, clang_codeCompleteGetDiagnostic_wrapper));
    exports.Set("clang_codeCompleteGetContexts", Napi::Function::New(env, clang_codeCompleteGetContexts_wrapper));
    exports.Set("clang_codeCompleteGetContainerKind", Napi::Function::New(env, clang_codeCompleteGetContainerKind_wrapper));
    exports.Set("clang_codeCompleteGetContainerUSR", Napi::Function::New(env, clang_codeCompleteGetContainerUSR_wrapper));
    exports.Set("clang_codeCompleteGetObjCSelector", Napi::Function::New(env, clang_codeCompleteGetObjCSelector_wrapper));
    exports.Set("clang_getClangVersion", Napi::Function::New(env, clang_getClangVersion_wrapper));
    exports.Set("clang_toggleCrashRecovery", Napi::Function::New(env, clang_toggleCrashRecovery_wrapper));
    exports.Set("clang_getInclusions", Napi::Function::New(env, clang_getInclusions_wrapper));
    exports.Set("clang_Cursor_Evaluate", Napi::Function::New(env, clang_Cursor_Evaluate_wrapper));
    exports.Set("clang_EvalResult_getKind", Napi::Function::New(env, clang_EvalResult_getKind_wrapper));
    exports.Set("clang_EvalResult_getAsInt", Napi::Function::New(env, clang_EvalResult_getAsInt_wrapper));
    exports.Set("clang_EvalResult_getAsLongLong", Napi::Function::New(env, clang_EvalResult_getAsLongLong_wrapper));
    exports.Set("clang_EvalResult_isUnsignedInt", Napi::Function::New(env, clang_EvalResult_isUnsignedInt_wrapper));
    exports.Set("clang_EvalResult_getAsUnsigned", Napi::Function::New(env, clang_EvalResult_getAsUnsigned_wrapper));
    exports.Set("clang_EvalResult_getAsDouble", Napi::Function::New(env, clang_EvalResult_getAsDouble_wrapper));
    exports.Set("clang_EvalResult_getAsStr", Napi::Function::New(env, clang_EvalResult_getAsStr_wrapper));
    exports.Set("clang_EvalResult_dispose", Napi::Function::New(env, clang_EvalResult_dispose_wrapper));
    exports.Set("clang_getRemappings", Napi::Function::New(env, clang_getRemappings_wrapper));
    exports.Set("clang_getRemappingsFromFileList", Napi::Function::New(env, clang_getRemappingsFromFileList_wrapper));
    exports.Set("clang_remap_getNumFiles", Napi::Function::New(env, clang_remap_getNumFiles_wrapper));
    exports.Set("clang_remap_getFilenames", Napi::Function::New(env, clang_remap_getFilenames_wrapper));
    exports.Set("clang_remap_dispose", Napi::Function::New(env, clang_remap_dispose_wrapper));
    exports.Set("clang_findReferencesInFile", Napi::Function::New(env, clang_findReferencesInFile_wrapper));
    exports.Set("clang_findIncludesInFile", Napi::Function::New(env, clang_findIncludesInFile_wrapper));
    exports.Set("clang_findReferencesInFileWithBlock", Napi::Function::New(env, clang_findReferencesInFileWithBlock_wrapper));
    exports.Set("clang_findIncludesInFileWithBlock", Napi::Function::New(env, clang_findIncludesInFileWithBlock_wrapper));
    exports.Set("clang_index_isEntityObjCContainerKind", Napi::Function::New(env, clang_index_isEntityObjCContainerKind_wrapper));
    exports.Set("clang_index_getObjCContainerDeclInfo", Napi::Function::New(env, clang_index_getObjCContainerDeclInfo_wrapper));
    exports.Set("clang_index_getObjCInterfaceDeclInfo", Napi::Function::New(env, clang_index_getObjCInterfaceDeclInfo_wrapper));
    exports.Set("clang_index_getObjCCategoryDeclInfo", Napi::Function::New(env, clang_index_getObjCCategoryDeclInfo_wrapper));
    exports.Set("clang_index_getObjCProtocolRefListInfo", Napi::Function::New(env, clang_index_getObjCProtocolRefListInfo_wrapper));
    exports.Set("clang_index_getObjCPropertyDeclInfo", Napi::Function::New(env, clang_index_getObjCPropertyDeclInfo_wrapper));
    exports.Set("clang_index_getIBOutletCollectionAttrInfo", Napi::Function::New(env, clang_index_getIBOutletCollectionAttrInfo_wrapper));
    exports.Set("clang_index_getCXXClassDeclInfo", Napi::Function::New(env, clang_index_getCXXClassDeclInfo_wrapper));
    exports.Set("clang_index_getClientContainer", Napi::Function::New(env, clang_index_getClientContainer_wrapper));
    exports.Set("clang_index_setClientContainer", Napi::Function::New(env, clang_index_setClientContainer_wrapper));
    exports.Set("clang_index_getClientEntity", Napi::Function::New(env, clang_index_getClientEntity_wrapper));
    exports.Set("clang_index_setClientEntity", Napi::Function::New(env, clang_index_setClientEntity_wrapper));
    exports.Set("clang_IndexAction_create", Napi::Function::New(env, clang_IndexAction_create_wrapper));
    exports.Set("clang_IndexAction_dispose", Napi::Function::New(env, clang_IndexAction_dispose_wrapper));
    exports.Set("clang_indexSourceFile", Napi::Function::New(env, clang_indexSourceFile_wrapper));
    exports.Set("clang_indexSourceFileFullArgv", Napi::Function::New(env, clang_indexSourceFileFullArgv_wrapper));
    exports.Set("clang_indexTranslationUnit", Napi::Function::New(env, clang_indexTranslationUnit_wrapper));
    exports.Set("clang_indexLoc_getFileLocation", Napi::Function::New(env, clang_indexLoc_getFileLocation_wrapper));
    exports.Set("clang_indexLoc_getCXSourceLocation", Napi::Function::New(env, clang_indexLoc_getCXSourceLocation_wrapper));
    exports.Set("clang_Type_visitFields", Napi::Function::New(env, clang_Type_visitFields_wrapper));
    exports.Set("clang_visitCXXBaseClasses", Napi::Function::New(env, clang_visitCXXBaseClasses_wrapper));
    exports.Set("clang_getBinaryOperatorKindSpelling", Napi::Function::New(env, clang_getBinaryOperatorKindSpelling_wrapper));
    exports.Set("clang_getCursorBinaryOperatorKind", Napi::Function::New(env, clang_getCursorBinaryOperatorKind_wrapper));
    exports.Set("clang_getUnaryOperatorKindSpelling", Napi::Function::New(env, clang_getUnaryOperatorKindSpelling_wrapper));
    exports.Set("clang_getCursorUnaryOperatorKind", Napi::Function::New(env, clang_getCursorUnaryOperatorKind_wrapper));
    return exports;
}

NODE_API_MODULE(node-clang-raw, Init)