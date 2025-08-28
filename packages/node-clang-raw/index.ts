import nodeGypBuild from 'node-gyp-build';

// Load the native addon
const addon = nodeGypBuild(import.meta.dirname) as any;

// Struct type definitions
export interface CXTargetInfoImpl {
  _ptr: unknown;
  _type: 'CXTargetInfoImpl';
}

export interface CXTargetInfoImplInit {
}

export interface CXTranslationUnitImpl {
  _ptr: unknown;
  _type: 'CXTranslationUnitImpl';
}

export interface CXTranslationUnitImplInit {
}

/**
 * Provides the contents of a file that has not yet been saved to disk.

Each CXUnsavedFile instance provides the name of a file on the
system along with the current contents of that file that have not
yet been saved to disk.
 */
export interface CXUnsavedFile {
  _ptr: unknown;
  _type: 'CXUnsavedFile';
}

export interface CXUnsavedFileInit {
  Filename?: string;
  Contents?: string;
  Length?: number;
}

/**
 * Describes a version number of the form major.minor.subminor.
 */
export interface CXVersion {
  _ptr: unknown;
  _type: 'CXVersion';
}

export interface CXVersionInit {
  Major?: number;
  Minor?: number;
  Subminor?: number;
}

/**
 * Index initialization options.

0 is the default value of each member of this struct except for Size.
Initialize the struct in one of the following three ways to avoid adapting
code each time a new member is added to it:
\code
CXIndexOptions Opts;
memset(&Opts, 0, sizeof(Opts));
Opts.Size = sizeof(CXIndexOptions);
\endcode
or explicitly initialize the first data member and zero-initialize the rest:
\code
CXIndexOptions Opts = { sizeof(CXIndexOptions) };
\endcode
or to prevent the -Wmissing-field-initializers warning for the above version:
\code
CXIndexOptions Opts{};
Opts.Size = sizeof(CXIndexOptions);
\endcode
 */
export interface CXIndexOptions {
  _ptr: unknown;
  _type: 'CXIndexOptions';
}

export interface CXIndexOptionsInit {
  Size?: number;
  ThreadBackgroundPriorityForIndexing?: number;
  ThreadBackgroundPriorityForEditing?: number;
  ExcludeDeclarationsFromPCH?: number;
  DisplayDiagnostics?: number;
  StorePreamblesInMemory?: number;
  PreambleStoragePath?: string;
  InvocationEmissionPath?: string;
}

export interface CXTUResourceUsageEntry {
  _ptr: unknown;
  _type: 'CXTUResourceUsageEntry';
}

export interface CXTUResourceUsageEntryInit {
  kind?: number;
  amount?: number;
}

/**
 * The memory usage of a CXTranslationUnit, broken into categories.
 */
export interface CXTUResourceUsage {
  _ptr: unknown;
  _type: 'CXTUResourceUsage';
}

export interface CXTUResourceUsageInit {
  data?: unknown;
  numEntries?: number;
  entries?: { _ptr: unknown };
}

/**
 * A cursor representing some element in the abstract syntax tree for
a translation unit.

The cursor abstraction unifies the different kinds of entities in a
program--declaration, statements, expressions, references to declarations,
etc.--under a single "cursor" abstraction with a common set of operations.
Common operation for a cursor include: getting the physical location in
a source file where the cursor points, getting the name associated with a
cursor, and retrieving cursors for any child nodes of a particular cursor.

Cursors can be produced in two specific ways.
clang_getTranslationUnitCursor() produces a cursor for a translation unit,
from which one can use clang_visitChildren() to explore the rest of the
translation unit. clang_getCursor() maps from a physical source location
to the entity that resides at that location, allowing one to map from the
source code into the AST.
 */
export interface CXCursor {
  _ptr: unknown;
  _type: 'CXCursor';
}

export interface CXCursorInit {
  kind?: number;
  xdata?: number;
  data?: unknown[];
}

/**
 * Describes the availability of a given entity on a particular platform, e.g.,
a particular class might only be available on Mac OS 10.7 or newer.
 */
export interface CXPlatformAvailability {
  _ptr: unknown;
  _type: 'CXPlatformAvailability';
}

export interface CXPlatformAvailabilityInit {
  Platform?: { _type: 'CXString' };
  Introduced?: { _type: 'CXVersion' };
  Deprecated?: { _type: 'CXVersion' };
  Obsoleted?: { _type: 'CXVersion' };
  Unavailable?: number;
  Message?: { _type: 'CXString' };
}

export interface CXCursorSetImpl {
  _ptr: unknown;
  _type: 'CXCursorSetImpl';
}

export interface CXCursorSetImplInit {
}

/**
 * The type of an element in the abstract syntax tree.
 */
export interface CXType {
  _ptr: unknown;
  _type: 'CXType';
}

export interface CXTypeInit {
  kind?: number;
  data?: unknown[];
}

/**
 * Describes a single preprocessing token.
 */
export interface CXToken {
  _ptr: unknown;
  _type: 'CXToken';
}

export interface CXTokenInit {
  int_data?: unknown[];
  ptr_data?: unknown;
}

/**
 * A single result of code completion.
 */
export interface CXCompletionResult {
  _ptr: unknown;
  _type: 'CXCompletionResult';
}

export interface CXCompletionResultInit {
  CursorKind?: number;
  CompletionString?: { _type: 'CXCompletionString' };
}

/**
 * Contains the results of code-completion.

This data structure contains the results of code completion, as
produced by \c clang_codeCompleteAt(). Its contents must be freed by
\c clang_disposeCodeCompleteResults.
 */
export interface CXCodeCompleteResults {
  _ptr: unknown;
  _type: 'CXCodeCompleteResults';
}

export interface CXCodeCompleteResultsInit {
  Results?: { _ptr: unknown };
  NumResults?: number;
}

export interface CXCursorAndRangeVisitor {
  _ptr: unknown;
  _type: 'CXCursorAndRangeVisitor';
}

export interface CXCursorAndRangeVisitorInit {
  context?: unknown;
  visit?: number;
}

/**
 * Source location passed to index callbacks.
 */
export interface CXIdxLoc {
  _ptr: unknown;
  _type: 'CXIdxLoc';
}

export interface CXIdxLocInit {
  ptr_data?: unknown[];
  int_data?: number;
}

/**
 * Data for ppIncludedFile callback.
 */
export interface CXIdxIncludedFileInfo {
  _ptr: unknown;
  _type: 'CXIdxIncludedFileInfo';
}

export interface CXIdxIncludedFileInfoInit {
  hashLoc?: { _type: 'CXIdxLoc' };
  filename?: string;
  file?: { _type: 'CXFile' };
  isImport?: number;
  isAngled?: number;
  isModuleImport?: number;
}

/**
 * Data for IndexerCallbacks#importedASTFile.
 */
export interface CXIdxImportedASTFileInfo {
  _ptr: unknown;
  _type: 'CXIdxImportedASTFileInfo';
}

export interface CXIdxImportedASTFileInfoInit {
  file?: { _type: 'CXFile' };
  module?: { _type: 'CXModule' };
  loc?: { _type: 'CXIdxLoc' };
  isImplicit?: number;
}

export interface CXIdxAttrInfo {
  _ptr: unknown;
  _type: 'CXIdxAttrInfo';
}

export interface CXIdxAttrInfoInit {
  kind?: { _type: 'CXIdxAttrKind' };
  cursor?: { _type: 'CXCursor' };
  loc?: { _type: 'CXIdxLoc' };
}

export interface CXIdxEntityInfo {
  _ptr: unknown;
  _type: 'CXIdxEntityInfo';
}

export interface CXIdxEntityInfoInit {
  kind?: { _type: 'CXIdxEntityKind' };
  templateKind?: { _type: 'CXIdxEntityCXXTemplateKind' };
  lang?: { _type: 'CXIdxEntityLanguage' };
  name?: string;
  USR?: string;
  cursor?: { _type: 'CXCursor' };
  attributes?: { _ptr: unknown };
  numAttributes?: number;
}

export interface CXIdxContainerInfo {
  _ptr: unknown;
  _type: 'CXIdxContainerInfo';
}

export interface CXIdxContainerInfoInit {
  cursor?: { _type: 'CXCursor' };
}

export interface CXIdxIBOutletCollectionAttrInfo {
  _ptr: unknown;
  _type: 'CXIdxIBOutletCollectionAttrInfo';
}

export interface CXIdxIBOutletCollectionAttrInfoInit {
  attrInfo?: { _ptr: unknown };
  objcClass?: { _ptr: unknown };
  classCursor?: { _type: 'CXCursor' };
  classLoc?: { _type: 'CXIdxLoc' };
}

export interface CXIdxDeclInfo {
  _ptr: unknown;
  _type: 'CXIdxDeclInfo';
}

export interface CXIdxDeclInfoInit {
  entityInfo?: { _ptr: unknown };
  cursor?: { _type: 'CXCursor' };
  loc?: { _type: 'CXIdxLoc' };
  semanticContainer?: { _ptr: unknown };
  lexicalContainer?: { _ptr: unknown };
  isRedeclaration?: number;
  isDefinition?: number;
  isContainer?: number;
  declAsContainer?: { _ptr: unknown };
  isImplicit?: number;
  attributes?: { _ptr: unknown };
  numAttributes?: number;
  flags?: number;
}

export interface CXIdxObjCContainerDeclInfo {
  _ptr: unknown;
  _type: 'CXIdxObjCContainerDeclInfo';
}

export interface CXIdxObjCContainerDeclInfoInit {
  declInfo?: { _ptr: unknown };
  kind?: { _type: 'CXIdxObjCContainerKind' };
}

export interface CXIdxBaseClassInfo {
  _ptr: unknown;
  _type: 'CXIdxBaseClassInfo';
}

export interface CXIdxBaseClassInfoInit {
  base?: { _ptr: unknown };
  cursor?: { _type: 'CXCursor' };
  loc?: { _type: 'CXIdxLoc' };
}

export interface CXIdxObjCProtocolRefInfo {
  _ptr: unknown;
  _type: 'CXIdxObjCProtocolRefInfo';
}

export interface CXIdxObjCProtocolRefInfoInit {
  protocol?: { _ptr: unknown };
  cursor?: { _type: 'CXCursor' };
  loc?: { _type: 'CXIdxLoc' };
}

export interface CXIdxObjCProtocolRefListInfo {
  _ptr: unknown;
  _type: 'CXIdxObjCProtocolRefListInfo';
}

export interface CXIdxObjCProtocolRefListInfoInit {
  protocols?: { _ptr: unknown };
  numProtocols?: number;
}

export interface CXIdxObjCInterfaceDeclInfo {
  _ptr: unknown;
  _type: 'CXIdxObjCInterfaceDeclInfo';
}

export interface CXIdxObjCInterfaceDeclInfoInit {
  containerInfo?: { _ptr: unknown };
  superInfo?: { _ptr: unknown };
  protocols?: { _ptr: unknown };
}

export interface CXIdxObjCCategoryDeclInfo {
  _ptr: unknown;
  _type: 'CXIdxObjCCategoryDeclInfo';
}

export interface CXIdxObjCCategoryDeclInfoInit {
  containerInfo?: { _ptr: unknown };
  objcClass?: { _ptr: unknown };
  classCursor?: { _type: 'CXCursor' };
  classLoc?: { _type: 'CXIdxLoc' };
  protocols?: { _ptr: unknown };
}

export interface CXIdxObjCPropertyDeclInfo {
  _ptr: unknown;
  _type: 'CXIdxObjCPropertyDeclInfo';
}

export interface CXIdxObjCPropertyDeclInfoInit {
  declInfo?: { _ptr: unknown };
  getter?: { _ptr: unknown };
  setter?: { _ptr: unknown };
}

export interface CXIdxCXXClassDeclInfo {
  _ptr: unknown;
  _type: 'CXIdxCXXClassDeclInfo';
}

export interface CXIdxCXXClassDeclInfoInit {
  declInfo?: { _ptr: unknown };
  bases?: { _ptr: unknown };
  numBases?: number;
}

/**
 * Data for IndexerCallbacks#indexEntityReference.
 */
export interface CXIdxEntityRefInfo {
  _ptr: unknown;
  _type: 'CXIdxEntityRefInfo';
}

export interface CXIdxEntityRefInfoInit {
  kind?: { _type: 'CXIdxEntityRefKind' };
  cursor?: { _type: 'CXCursor' };
  loc?: { _type: 'CXIdxLoc' };
  referencedEntity?: { _ptr: unknown };
  parentEntity?: { _ptr: unknown };
  container?: { _ptr: unknown };
  role?: { _type: 'CXSymbolRole' };
}

/**
 * A group of callbacks used by #clang_indexSourceFile and
#clang_indexTranslationUnit.
 */
export interface IndexerCallbacks {
  _ptr: unknown;
  _type: 'IndexerCallbacks';
}

export interface IndexerCallbacksInit {
  abortQuery?: { _type: 'int (*)(CXClientData, void *)' };
  diagnostic?: { _type: 'void (*)(CXClientData, CXDiagnosticSet, void *)' };
  enteredMainFile?: { _type: 'CXIdxClientFile (*)(CXClientData, CXFile, void *)' };
  ppIncludedFile?: { _type: 'CXIdxClientFile (*)(CXClientData, const CXIdxIncludedFileInfo *)' };
  importedASTFile?: { _type: 'CXIdxClientASTFile (*)(CXClientData, const CXIdxImportedASTFileInfo *)' };
  startedTranslationUnit?: { _type: 'CXIdxClientContainer (*)(CXClientData, void *)' };
  indexDeclaration?: { _type: 'void (*)(CXClientData, const CXIdxDeclInfo *)' };
  indexEntityReference?: { _type: 'void (*)(CXClientData, const CXIdxEntityRefInfo *)' };
}

/**
 * A character string.

The \c CXString type is used to return strings from the interface when
the ownership of that string might differ from one call to the next.
Use \c clang_getCString() to retrieve the string data and, once finished
with the string data, call \c clang_disposeString() to free the string.
 */
export interface CXString {
  _ptr: unknown;
  _type: 'CXString';
}

export interface CXStringInit {
  data?: unknown;
  private_flags?: number;
}

export interface CXStringSet {
  _ptr: unknown;
  _type: 'CXStringSet';
}

export interface CXStringSetInit {
  Strings?: { _ptr: unknown };
  Count?: number;
}

/**
 * Identifies a specific source location within a translation
unit.

Use clang_getExpansionLocation() or clang_getSpellingLocation()
to map a source location to a particular file, line, and column.
 */
export interface CXSourceLocation {
  _ptr: unknown;
  _type: 'CXSourceLocation';
}

export interface CXSourceLocationInit {
  ptr_data?: unknown[];
  int_data?: number;
}

/**
 * Identifies a half-open character range in the source code.

Use clang_getRangeStart() and clang_getRangeEnd() to retrieve the
starting and end locations from a source range, respectively.
 */
export interface CXSourceRange {
  _ptr: unknown;
  _type: 'CXSourceRange';
}

export interface CXSourceRangeInit {
  ptr_data?: unknown[];
  begin_int_data?: number;
  end_int_data?: number;
}

/**
 * Identifies an array of ranges.
 */
export interface CXSourceRangeList {
  _ptr: unknown;
  _type: 'CXSourceRangeList';
}

export interface CXSourceRangeListInit {
  count?: number;
  ranges?: { _ptr: unknown };
}


// Enum constants
/**
 * Describes the availability of a particular entity, which indicates
whether the use of this entity will result in a warning or error due to
it being deprecated or unavailable.
 */
export const CXAvailabilityKind = {
  /** The entity is available. */
  CXAvailability_Available: 0,
  /** The entity is available, but has been deprecated (and its use is
not recommended). */
  CXAvailability_Deprecated: 1,
  /** The entity is not available; any use of it will be an error. */
  CXAvailability_NotAvailable: 2,
  /** The entity is available, but not accessible; any use of it will be
an error. */
  CXAvailability_NotAccessible: 3,
} as const;

/** The entity is available. */
export const CXAvailability_Available: number = CXAvailabilityKind.CXAvailability_Available;
/** The entity is available, but has been deprecated (and its use is
not recommended). */
export const CXAvailability_Deprecated: number = CXAvailabilityKind.CXAvailability_Deprecated;
/** The entity is not available; any use of it will be an error. */
export const CXAvailability_NotAvailable: number = CXAvailabilityKind.CXAvailability_NotAvailable;
/** The entity is available, but not accessible; any use of it will be
an error. */
export const CXAvailability_NotAccessible: number = CXAvailabilityKind.CXAvailability_NotAccessible;

/**
 * Describes the exception specification of a cursor.

A negative value indicates that the cursor is not a function declaration.
 */
export const CXCursor_ExceptionSpecificationKind = {
  /** The cursor has no exception specification. */
  CXCursor_ExceptionSpecificationKind_None: 0,
  /** The cursor has exception specification throw() */
  CXCursor_ExceptionSpecificationKind_DynamicNone: 1,
  /** The cursor has exception specification throw(T1, T2) */
  CXCursor_ExceptionSpecificationKind_Dynamic: 2,
  /** The cursor has exception specification throw(...). */
  CXCursor_ExceptionSpecificationKind_MSAny: 3,
  /** The cursor has exception specification basic noexcept. */
  CXCursor_ExceptionSpecificationKind_BasicNoexcept: 4,
  /** The cursor has exception specification computed noexcept. */
  CXCursor_ExceptionSpecificationKind_ComputedNoexcept: 5,
  /** The exception specification has not yet been evaluated. */
  CXCursor_ExceptionSpecificationKind_Unevaluated: 6,
  /** The exception specification has not yet been instantiated. */
  CXCursor_ExceptionSpecificationKind_Uninstantiated: 7,
  /** The exception specification has not been parsed yet. */
  CXCursor_ExceptionSpecificationKind_Unparsed: 8,
  /** The cursor has a __declspec(nothrow) exception specification. */
  CXCursor_ExceptionSpecificationKind_NoThrow: 9,
} as const;

/** The cursor has no exception specification. */
export const CXCursor_ExceptionSpecificationKind_None: number = CXCursor_ExceptionSpecificationKind.CXCursor_ExceptionSpecificationKind_None;
/** The cursor has exception specification throw() */
export const CXCursor_ExceptionSpecificationKind_DynamicNone: number = CXCursor_ExceptionSpecificationKind.CXCursor_ExceptionSpecificationKind_DynamicNone;
/** The cursor has exception specification throw(T1, T2) */
export const CXCursor_ExceptionSpecificationKind_Dynamic: number = CXCursor_ExceptionSpecificationKind.CXCursor_ExceptionSpecificationKind_Dynamic;
/** The cursor has exception specification throw(...). */
export const CXCursor_ExceptionSpecificationKind_MSAny: number = CXCursor_ExceptionSpecificationKind.CXCursor_ExceptionSpecificationKind_MSAny;
/** The cursor has exception specification basic noexcept. */
export const CXCursor_ExceptionSpecificationKind_BasicNoexcept: number = CXCursor_ExceptionSpecificationKind.CXCursor_ExceptionSpecificationKind_BasicNoexcept;
/** The cursor has exception specification computed noexcept. */
export const CXCursor_ExceptionSpecificationKind_ComputedNoexcept: number = CXCursor_ExceptionSpecificationKind.CXCursor_ExceptionSpecificationKind_ComputedNoexcept;
/** The exception specification has not yet been evaluated. */
export const CXCursor_ExceptionSpecificationKind_Unevaluated: number = CXCursor_ExceptionSpecificationKind.CXCursor_ExceptionSpecificationKind_Unevaluated;
/** The exception specification has not yet been instantiated. */
export const CXCursor_ExceptionSpecificationKind_Uninstantiated: number = CXCursor_ExceptionSpecificationKind.CXCursor_ExceptionSpecificationKind_Uninstantiated;
/** The exception specification has not been parsed yet. */
export const CXCursor_ExceptionSpecificationKind_Unparsed: number = CXCursor_ExceptionSpecificationKind.CXCursor_ExceptionSpecificationKind_Unparsed;
/** The cursor has a __declspec(nothrow) exception specification. */
export const CXCursor_ExceptionSpecificationKind_NoThrow: number = CXCursor_ExceptionSpecificationKind.CXCursor_ExceptionSpecificationKind_NoThrow;

export const CXChoice = {
  /** Use the default value of an option that may depend on the process
environment. */
  CXChoice_Default: 0,
  /** Enable the option. */
  CXChoice_Enabled: 1,
  /** Disable the option. */
  CXChoice_Disabled: 2,
} as const;

/** Use the default value of an option that may depend on the process
environment. */
export const CXChoice_Default: number = CXChoice.CXChoice_Default;
/** Enable the option. */
export const CXChoice_Enabled: number = CXChoice.CXChoice_Enabled;
/** Disable the option. */
export const CXChoice_Disabled: number = CXChoice.CXChoice_Disabled;

export const CXGlobalOptFlags = {
  /** Used to indicate that no special CXIndex options are needed. */
  CXGlobalOpt_None: 0,
  /** Used to indicate that threads that libclang creates for indexing
purposes should use background priority.

Affects #clang_indexSourceFile, #clang_indexTranslationUnit,
#clang_parseTranslationUnit, #clang_saveTranslationUnit. */
  CXGlobalOpt_ThreadBackgroundPriorityForIndexing: 1,
  /** Used to indicate that threads that libclang creates for editing
purposes should use background priority.

Affects #clang_reparseTranslationUnit, #clang_codeCompleteAt,
#clang_annotateTokens */
  CXGlobalOpt_ThreadBackgroundPriorityForEditing: 2,
  /** Used to indicate that all threads that libclang creates should use
background priority. */
  CXGlobalOpt_ThreadBackgroundPriorityForAll: 3,
} as const;

/** Used to indicate that no special CXIndex options are needed. */
export const CXGlobalOpt_None: number = CXGlobalOptFlags.CXGlobalOpt_None;
/** Used to indicate that threads that libclang creates for indexing
purposes should use background priority.

Affects #clang_indexSourceFile, #clang_indexTranslationUnit,
#clang_parseTranslationUnit, #clang_saveTranslationUnit. */
export const CXGlobalOpt_ThreadBackgroundPriorityForIndexing: number = CXGlobalOptFlags.CXGlobalOpt_ThreadBackgroundPriorityForIndexing;
/** Used to indicate that threads that libclang creates for editing
purposes should use background priority.

Affects #clang_reparseTranslationUnit, #clang_codeCompleteAt,
#clang_annotateTokens */
export const CXGlobalOpt_ThreadBackgroundPriorityForEditing: number = CXGlobalOptFlags.CXGlobalOpt_ThreadBackgroundPriorityForEditing;
/** Used to indicate that all threads that libclang creates should use
background priority. */
export const CXGlobalOpt_ThreadBackgroundPriorityForAll: number = CXGlobalOptFlags.CXGlobalOpt_ThreadBackgroundPriorityForAll;

/**
 * Flags that control the creation of translation units.

The enumerators in this enumeration type are meant to be bitwise
ORed together to specify which options should be used when
constructing the translation unit.
 */
export const CXTranslationUnit_Flags = {
  /** Used to indicate that no special translation-unit options are
needed. */
  CXTranslationUnit_None: 0,
  /** Used to indicate that the parser should construct a "detailed"
preprocessing record, including all macro definitions and instantiations.

Constructing a detailed preprocessing record requires more memory
and time to parse, since the information contained in the record
is usually not retained. However, it can be useful for
applications that require more detailed information about the
behavior of the preprocessor. */
  CXTranslationUnit_DetailedPreprocessingRecord: 1,
  /** Used to indicate that the translation unit is incomplete.

When a translation unit is considered "incomplete", semantic
analysis that is typically performed at the end of the
translation unit will be suppressed. For example, this suppresses
the completion of tentative declarations in C and of
instantiation of implicitly-instantiation function templates in
C++. This option is typically used when parsing a header with the
intent of producing a precompiled header. */
  CXTranslationUnit_Incomplete: 2,
  /** Used to indicate that the translation unit should be built with an
implicit precompiled header for the preamble.

An implicit precompiled header is used as an optimization when a
particular translation unit is likely to be reparsed many times
when the sources aren't changing that often. In this case, an
implicit precompiled header will be built containing all of the
initial includes at the top of the main file (what we refer to as
the "preamble" of the file). In subsequent parses, if the
preamble or the files in it have not changed, \c
clang_reparseTranslationUnit() will re-use the implicit
precompiled header to improve parsing performance. */
  CXTranslationUnit_PrecompiledPreamble: 4,
  /** Used to indicate that the translation unit should cache some
code-completion results with each reparse of the source file.

Caching of code-completion results is a performance optimization that
introduces some overhead to reparsing but improves the performance of
code-completion operations. */
  CXTranslationUnit_CacheCompletionResults: 8,
  /** Used to indicate that the translation unit will be serialized with
\c clang_saveTranslationUnit.

This option is typically used when parsing a header with the intent of
producing a precompiled header. */
  CXTranslationUnit_ForSerialization: 16,
  /** DEPRECATED: Enabled chained precompiled preambles in C++.

Note: this is a *temporary* option that is available only while
we are testing C++ precompiled preamble support. It is deprecated. */
  CXTranslationUnit_CXXChainedPCH: 32,
  /** Used to indicate that function/method bodies should be skipped while
parsing.

This option can be used to search for declarations/definitions while
ignoring the usages. */
  CXTranslationUnit_SkipFunctionBodies: 64,
  /** Used to indicate that brief documentation comments should be
included into the set of code completions returned from this translation
unit. */
  CXTranslationUnit_IncludeBriefCommentsInCodeCompletion: 128,
  /** Used to indicate that the precompiled preamble should be created on
the first parse. Otherwise it will be created on the first reparse. This
trades runtime on the first parse (serializing the preamble takes time) for
reduced runtime on the second parse (can now reuse the preamble). */
  CXTranslationUnit_CreatePreambleOnFirstParse: 256,
  /** Do not stop processing when fatal errors are encountered.

When fatal errors are encountered while parsing a translation unit,
semantic analysis is typically stopped early when compiling code. A common
source for fatal errors are unresolvable include files. For the
purposes of an IDE, this is undesirable behavior and as much information
as possible should be reported. Use this flag to enable this behavior. */
  CXTranslationUnit_KeepGoing: 512,
  /** Sets the preprocessor in a mode for parsing a single file only. */
  CXTranslationUnit_SingleFileParse: 1024,
  /** Used in combination with CXTranslationUnit_SkipFunctionBodies to
constrain the skipping of function bodies to the preamble.

The function bodies of the main file are not skipped. */
  CXTranslationUnit_LimitSkipFunctionBodiesToPreamble: 2048,
  /** Used to indicate that attributed types should be included in CXType. */
  CXTranslationUnit_IncludeAttributedTypes: 4096,
  /** Used to indicate that implicit attributes should be visited. */
  CXTranslationUnit_VisitImplicitAttributes: 8192,
  /** Used to indicate that non-errors from included files should be ignored.

If set, clang_getDiagnosticSetFromTU() will not report e.g. warnings from
included files anymore. This speeds up clang_getDiagnosticSetFromTU() for
the case where these warnings are not of interest, as for an IDE for
example, which typically shows only the diagnostics in the main file. */
  CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles: 16384,
  /** Tells the preprocessor not to skip excluded conditional blocks. */
  CXTranslationUnit_RetainExcludedConditionalBlocks: 32768,
} as const;

/** Used to indicate that no special translation-unit options are
needed. */
export const CXTranslationUnit_None: number = CXTranslationUnit_Flags.CXTranslationUnit_None;
/** Used to indicate that the parser should construct a "detailed"
preprocessing record, including all macro definitions and instantiations.

Constructing a detailed preprocessing record requires more memory
and time to parse, since the information contained in the record
is usually not retained. However, it can be useful for
applications that require more detailed information about the
behavior of the preprocessor. */
export const CXTranslationUnit_DetailedPreprocessingRecord: number = CXTranslationUnit_Flags.CXTranslationUnit_DetailedPreprocessingRecord;
/** Used to indicate that the translation unit is incomplete.

When a translation unit is considered "incomplete", semantic
analysis that is typically performed at the end of the
translation unit will be suppressed. For example, this suppresses
the completion of tentative declarations in C and of
instantiation of implicitly-instantiation function templates in
C++. This option is typically used when parsing a header with the
intent of producing a precompiled header. */
export const CXTranslationUnit_Incomplete: number = CXTranslationUnit_Flags.CXTranslationUnit_Incomplete;
/** Used to indicate that the translation unit should be built with an
implicit precompiled header for the preamble.

An implicit precompiled header is used as an optimization when a
particular translation unit is likely to be reparsed many times
when the sources aren't changing that often. In this case, an
implicit precompiled header will be built containing all of the
initial includes at the top of the main file (what we refer to as
the "preamble" of the file). In subsequent parses, if the
preamble or the files in it have not changed, \c
clang_reparseTranslationUnit() will re-use the implicit
precompiled header to improve parsing performance. */
export const CXTranslationUnit_PrecompiledPreamble: number = CXTranslationUnit_Flags.CXTranslationUnit_PrecompiledPreamble;
/** Used to indicate that the translation unit should cache some
code-completion results with each reparse of the source file.

Caching of code-completion results is a performance optimization that
introduces some overhead to reparsing but improves the performance of
code-completion operations. */
export const CXTranslationUnit_CacheCompletionResults: number = CXTranslationUnit_Flags.CXTranslationUnit_CacheCompletionResults;
/** Used to indicate that the translation unit will be serialized with
\c clang_saveTranslationUnit.

This option is typically used when parsing a header with the intent of
producing a precompiled header. */
export const CXTranslationUnit_ForSerialization: number = CXTranslationUnit_Flags.CXTranslationUnit_ForSerialization;
/** DEPRECATED: Enabled chained precompiled preambles in C++.

Note: this is a *temporary* option that is available only while
we are testing C++ precompiled preamble support. It is deprecated. */
export const CXTranslationUnit_CXXChainedPCH: number = CXTranslationUnit_Flags.CXTranslationUnit_CXXChainedPCH;
/** Used to indicate that function/method bodies should be skipped while
parsing.

This option can be used to search for declarations/definitions while
ignoring the usages. */
export const CXTranslationUnit_SkipFunctionBodies: number = CXTranslationUnit_Flags.CXTranslationUnit_SkipFunctionBodies;
/** Used to indicate that brief documentation comments should be
included into the set of code completions returned from this translation
unit. */
export const CXTranslationUnit_IncludeBriefCommentsInCodeCompletion: number = CXTranslationUnit_Flags.CXTranslationUnit_IncludeBriefCommentsInCodeCompletion;
/** Used to indicate that the precompiled preamble should be created on
the first parse. Otherwise it will be created on the first reparse. This
trades runtime on the first parse (serializing the preamble takes time) for
reduced runtime on the second parse (can now reuse the preamble). */
export const CXTranslationUnit_CreatePreambleOnFirstParse: number = CXTranslationUnit_Flags.CXTranslationUnit_CreatePreambleOnFirstParse;
/** Do not stop processing when fatal errors are encountered.

When fatal errors are encountered while parsing a translation unit,
semantic analysis is typically stopped early when compiling code. A common
source for fatal errors are unresolvable include files. For the
purposes of an IDE, this is undesirable behavior and as much information
as possible should be reported. Use this flag to enable this behavior. */
export const CXTranslationUnit_KeepGoing: number = CXTranslationUnit_Flags.CXTranslationUnit_KeepGoing;
/** Sets the preprocessor in a mode for parsing a single file only. */
export const CXTranslationUnit_SingleFileParse: number = CXTranslationUnit_Flags.CXTranslationUnit_SingleFileParse;
/** Used in combination with CXTranslationUnit_SkipFunctionBodies to
constrain the skipping of function bodies to the preamble.

The function bodies of the main file are not skipped. */
export const CXTranslationUnit_LimitSkipFunctionBodiesToPreamble: number = CXTranslationUnit_Flags.CXTranslationUnit_LimitSkipFunctionBodiesToPreamble;
/** Used to indicate that attributed types should be included in CXType. */
export const CXTranslationUnit_IncludeAttributedTypes: number = CXTranslationUnit_Flags.CXTranslationUnit_IncludeAttributedTypes;
/** Used to indicate that implicit attributes should be visited. */
export const CXTranslationUnit_VisitImplicitAttributes: number = CXTranslationUnit_Flags.CXTranslationUnit_VisitImplicitAttributes;
/** Used to indicate that non-errors from included files should be ignored.

If set, clang_getDiagnosticSetFromTU() will not report e.g. warnings from
included files anymore. This speeds up clang_getDiagnosticSetFromTU() for
the case where these warnings are not of interest, as for an IDE for
example, which typically shows only the diagnostics in the main file. */
export const CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles: number = CXTranslationUnit_Flags.CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles;
/** Tells the preprocessor not to skip excluded conditional blocks. */
export const CXTranslationUnit_RetainExcludedConditionalBlocks: number = CXTranslationUnit_Flags.CXTranslationUnit_RetainExcludedConditionalBlocks;

/**
 * Flags that control how translation units are saved.

The enumerators in this enumeration type are meant to be bitwise
ORed together to specify which options should be used when
saving the translation unit.
 */
export const CXSaveTranslationUnit_Flags = {
  /** Used to indicate that no special saving options are needed. */
  CXSaveTranslationUnit_None: 0,
} as const;

/** Used to indicate that no special saving options are needed. */
export const CXSaveTranslationUnit_None: number = CXSaveTranslationUnit_Flags.CXSaveTranslationUnit_None;

/**
 * Describes the kind of error that occurred (if any) in a call to
\c clang_saveTranslationUnit().
 */
export const CXSaveError = {
  /** Indicates that no error occurred while saving a translation unit. */
  CXSaveError_None: 0,
  /** Indicates that an unknown error occurred while attempting to save
the file.

This error typically indicates that file I/O failed when attempting to
write the file. */
  CXSaveError_Unknown: 1,
  /** Indicates that errors during translation prevented this attempt
to save the translation unit.

Errors that prevent the translation unit from being saved can be
extracted using \c clang_getNumDiagnostics() and \c clang_getDiagnostic(). */
  CXSaveError_TranslationErrors: 2,
  /** Indicates that the translation unit to be saved was somehow
invalid (e.g., NULL). */
  CXSaveError_InvalidTU: 3,
} as const;

/** Indicates that no error occurred while saving a translation unit. */
export const CXSaveError_None: number = CXSaveError.CXSaveError_None;
/** Indicates that an unknown error occurred while attempting to save
the file.

This error typically indicates that file I/O failed when attempting to
write the file. */
export const CXSaveError_Unknown: number = CXSaveError.CXSaveError_Unknown;
/** Indicates that errors during translation prevented this attempt
to save the translation unit.

Errors that prevent the translation unit from being saved can be
extracted using \c clang_getNumDiagnostics() and \c clang_getDiagnostic(). */
export const CXSaveError_TranslationErrors: number = CXSaveError.CXSaveError_TranslationErrors;
/** Indicates that the translation unit to be saved was somehow
invalid (e.g., NULL). */
export const CXSaveError_InvalidTU: number = CXSaveError.CXSaveError_InvalidTU;

/**
 * Flags that control the reparsing of translation units.

The enumerators in this enumeration type are meant to be bitwise
ORed together to specify which options should be used when
reparsing the translation unit.
 */
export const CXReparse_Flags = {
  /** Used to indicate that no special reparsing options are needed. */
  CXReparse_None: 0,
} as const;

/** Used to indicate that no special reparsing options are needed. */
export const CXReparse_None: number = CXReparse_Flags.CXReparse_None;

/**
 * Categorizes how memory is being used by a translation unit.
 */
export const CXTUResourceUsageKind = {
  CXTUResourceUsage_AST: 1,
  CXTUResourceUsage_Identifiers: 2,
  CXTUResourceUsage_Selectors: 3,
  CXTUResourceUsage_GlobalCompletionResults: 4,
  CXTUResourceUsage_SourceManagerContentCache: 5,
  CXTUResourceUsage_AST_SideTables: 6,
  CXTUResourceUsage_SourceManager_Membuffer_Malloc: 7,
  CXTUResourceUsage_SourceManager_Membuffer_MMap: 8,
  CXTUResourceUsage_ExternalASTSource_Membuffer_Malloc: 9,
  CXTUResourceUsage_ExternalASTSource_Membuffer_MMap: 10,
  CXTUResourceUsage_Preprocessor: 11,
  CXTUResourceUsage_PreprocessingRecord: 12,
  CXTUResourceUsage_SourceManager_DataStructures: 13,
  CXTUResourceUsage_Preprocessor_HeaderSearch: 14,
  CXTUResourceUsage_MEMORY_IN_BYTES_BEGIN: 1,
  CXTUResourceUsage_MEMORY_IN_BYTES_END: 14,
  CXTUResourceUsage_First: 1,
  CXTUResourceUsage_Last: 14,
} as const;

export const CXTUResourceUsage_AST: number = CXTUResourceUsageKind.CXTUResourceUsage_AST;
export const CXTUResourceUsage_Identifiers: number = CXTUResourceUsageKind.CXTUResourceUsage_Identifiers;
export const CXTUResourceUsage_Selectors: number = CXTUResourceUsageKind.CXTUResourceUsage_Selectors;
export const CXTUResourceUsage_GlobalCompletionResults: number = CXTUResourceUsageKind.CXTUResourceUsage_GlobalCompletionResults;
export const CXTUResourceUsage_SourceManagerContentCache: number = CXTUResourceUsageKind.CXTUResourceUsage_SourceManagerContentCache;
export const CXTUResourceUsage_AST_SideTables: number = CXTUResourceUsageKind.CXTUResourceUsage_AST_SideTables;
export const CXTUResourceUsage_SourceManager_Membuffer_Malloc: number = CXTUResourceUsageKind.CXTUResourceUsage_SourceManager_Membuffer_Malloc;
export const CXTUResourceUsage_SourceManager_Membuffer_MMap: number = CXTUResourceUsageKind.CXTUResourceUsage_SourceManager_Membuffer_MMap;
export const CXTUResourceUsage_ExternalASTSource_Membuffer_Malloc: number = CXTUResourceUsageKind.CXTUResourceUsage_ExternalASTSource_Membuffer_Malloc;
export const CXTUResourceUsage_ExternalASTSource_Membuffer_MMap: number = CXTUResourceUsageKind.CXTUResourceUsage_ExternalASTSource_Membuffer_MMap;
export const CXTUResourceUsage_Preprocessor: number = CXTUResourceUsageKind.CXTUResourceUsage_Preprocessor;
export const CXTUResourceUsage_PreprocessingRecord: number = CXTUResourceUsageKind.CXTUResourceUsage_PreprocessingRecord;
export const CXTUResourceUsage_SourceManager_DataStructures: number = CXTUResourceUsageKind.CXTUResourceUsage_SourceManager_DataStructures;
export const CXTUResourceUsage_Preprocessor_HeaderSearch: number = CXTUResourceUsageKind.CXTUResourceUsage_Preprocessor_HeaderSearch;
export const CXTUResourceUsage_MEMORY_IN_BYTES_BEGIN: number = CXTUResourceUsageKind.CXTUResourceUsage_MEMORY_IN_BYTES_BEGIN;
export const CXTUResourceUsage_MEMORY_IN_BYTES_END: number = CXTUResourceUsageKind.CXTUResourceUsage_MEMORY_IN_BYTES_END;
export const CXTUResourceUsage_First: number = CXTUResourceUsageKind.CXTUResourceUsage_First;
export const CXTUResourceUsage_Last: number = CXTUResourceUsageKind.CXTUResourceUsage_Last;

/**
 * Describes the kind of entity that a cursor refers to.
 */
export const CXCursorKind = {
  /** A declaration whose specific kind is not exposed via this
interface.

Unexposed declarations have the same operations as any other kind
of declaration; one can extract their location information,
spelling, find their definitions, etc. However, the specific kind
of the declaration is not reported. */
  CXCursor_UnexposedDecl: 1,
  /** A C or C++ struct. */
  CXCursor_StructDecl: 2,
  /** A C or C++ union. */
  CXCursor_UnionDecl: 3,
  /** A C++ class. */
  CXCursor_ClassDecl: 4,
  /** An enumeration. */
  CXCursor_EnumDecl: 5,
  /** A field (in C) or non-static data member (in C++) in a
struct, union, or C++ class. */
  CXCursor_FieldDecl: 6,
  /** An enumerator constant. */
  CXCursor_EnumConstantDecl: 7,
  /** A function. */
  CXCursor_FunctionDecl: 8,
  /** A variable. */
  CXCursor_VarDecl: 9,
  /** A function or method parameter. */
  CXCursor_ParmDecl: 10,
  /** An Objective-C \@interface. */
  CXCursor_ObjCInterfaceDecl: 11,
  /** An Objective-C \@interface for a category. */
  CXCursor_ObjCCategoryDecl: 12,
  /** An Objective-C \@protocol declaration. */
  CXCursor_ObjCProtocolDecl: 13,
  /** An Objective-C \@property declaration. */
  CXCursor_ObjCPropertyDecl: 14,
  /** An Objective-C instance variable. */
  CXCursor_ObjCIvarDecl: 15,
  /** An Objective-C instance method. */
  CXCursor_ObjCInstanceMethodDecl: 16,
  /** An Objective-C class method. */
  CXCursor_ObjCClassMethodDecl: 17,
  /** An Objective-C \@implementation. */
  CXCursor_ObjCImplementationDecl: 18,
  /** An Objective-C \@implementation for a category. */
  CXCursor_ObjCCategoryImplDecl: 19,
  /** A typedef. */
  CXCursor_TypedefDecl: 20,
  /** A C++ class method. */
  CXCursor_CXXMethod: 21,
  /** A C++ namespace. */
  CXCursor_Namespace: 22,
  /** A linkage specification, e.g. 'extern "C"'. */
  CXCursor_LinkageSpec: 23,
  /** A C++ constructor. */
  CXCursor_Constructor: 24,
  /** A C++ destructor. */
  CXCursor_Destructor: 25,
  /** A C++ conversion function. */
  CXCursor_ConversionFunction: 26,
  /** A C++ template type parameter. */
  CXCursor_TemplateTypeParameter: 27,
  /** A C++ non-type template parameter. */
  CXCursor_NonTypeTemplateParameter: 28,
  /** A C++ template template parameter. */
  CXCursor_TemplateTemplateParameter: 29,
  /** A C++ function template. */
  CXCursor_FunctionTemplate: 30,
  /** A C++ class template. */
  CXCursor_ClassTemplate: 31,
  /** A C++ class template partial specialization. */
  CXCursor_ClassTemplatePartialSpecialization: 32,
  /** A C++ namespace alias declaration. */
  CXCursor_NamespaceAlias: 33,
  /** A C++ using directive. */
  CXCursor_UsingDirective: 34,
  /** A C++ using declaration. */
  CXCursor_UsingDeclaration: 35,
  /** A C++ alias declaration */
  CXCursor_TypeAliasDecl: 36,
  /** An Objective-C \@synthesize definition. */
  CXCursor_ObjCSynthesizeDecl: 37,
  /** An Objective-C \@dynamic definition. */
  CXCursor_ObjCDynamicDecl: 38,
  /** An access specifier. */
  CXCursor_CXXAccessSpecifier: 39,
  /** An access specifier. */
  CXCursor_FirstDecl: 1,
  /** An access specifier. */
  CXCursor_LastDecl: 39,
  /** An access specifier. */
  CXCursor_FirstRef: 40,
  /** An access specifier. */
  CXCursor_ObjCSuperClassRef: 40,
  /** An access specifier. */
  CXCursor_ObjCProtocolRef: 41,
  /** An access specifier. */
  CXCursor_ObjCClassRef: 42,
  /** A reference to a type declaration.

A type reference occurs anywhere where a type is named but not
declared. For example, given:

\code
typedef unsigned size_type;
size_type size;
\endcode

The typedef is a declaration of size_type (CXCursor_TypedefDecl),
while the type of the variable "size" is referenced. The cursor
referenced by the type of size is the typedef for size_type. */
  CXCursor_TypeRef: 43,
  /** A reference to a type declaration.

A type reference occurs anywhere where a type is named but not
declared. For example, given:

\code
typedef unsigned size_type;
size_type size;
\endcode

The typedef is a declaration of size_type (CXCursor_TypedefDecl),
while the type of the variable "size" is referenced. The cursor
referenced by the type of size is the typedef for size_type. */
  CXCursor_CXXBaseSpecifier: 44,
  /** A reference to a class template, function template, template
template parameter, or class template partial specialization. */
  CXCursor_TemplateRef: 45,
  /** A reference to a namespace or namespace alias. */
  CXCursor_NamespaceRef: 46,
  /** A reference to a member of a struct, union, or class that occurs in
some non-expression context, e.g., a designated initializer. */
  CXCursor_MemberRef: 47,
  /** A reference to a labeled statement.

This cursor kind is used to describe the jump to "start_over" in the
goto statement in the following example:

\code
start_over:
++counter;

goto start_over;
\endcode

A label reference cursor refers to a label statement. */
  CXCursor_LabelRef: 48,
  /** A reference to a set of overloaded functions or function templates
that has not yet been resolved to a specific function or function template.

An overloaded declaration reference cursor occurs in C++ templates where
a dependent name refers to a function. For example:

\code
template<typename T> void swap(T&, T&);

struct X { ... };
void swap(X&, X&);

template<typename T>
void reverse(T* first, T* last) {
while (first < last - 1) {
swap(*first, *--last);
++first;
}
}

struct Y { };
void swap(Y&, Y&);
\endcode

Here, the identifier "swap" is associated with an overloaded declaration
reference. In the template definition, "swap" refers to either of the two
"swap" functions declared above, so both results will be available. At
instantiation time, "swap" may also refer to other functions found via
argument-dependent lookup (e.g., the "swap" function at the end of the
example).

The functions \c clang_getNumOverloadedDecls() and
\c clang_getOverloadedDecl() can be used to retrieve the definitions
referenced by this cursor. */
  CXCursor_OverloadedDeclRef: 49,
  /** A reference to a variable that occurs in some non-expression
context, e.g., a C++ lambda capture list. */
  CXCursor_VariableRef: 50,
  /** A reference to a variable that occurs in some non-expression
context, e.g., a C++ lambda capture list. */
  CXCursor_LastRef: 50,
  /** A reference to a variable that occurs in some non-expression
context, e.g., a C++ lambda capture list. */
  CXCursor_FirstInvalid: 70,
  /** A reference to a variable that occurs in some non-expression
context, e.g., a C++ lambda capture list. */
  CXCursor_InvalidFile: 70,
  /** A reference to a variable that occurs in some non-expression
context, e.g., a C++ lambda capture list. */
  CXCursor_NoDeclFound: 71,
  /** A reference to a variable that occurs in some non-expression
context, e.g., a C++ lambda capture list. */
  CXCursor_NotImplemented: 72,
  /** A reference to a variable that occurs in some non-expression
context, e.g., a C++ lambda capture list. */
  CXCursor_InvalidCode: 73,
  /** A reference to a variable that occurs in some non-expression
context, e.g., a C++ lambda capture list. */
  CXCursor_LastInvalid: 73,
  /** A reference to a variable that occurs in some non-expression
context, e.g., a C++ lambda capture list. */
  CXCursor_FirstExpr: 100,
  /** An expression whose specific kind is not exposed via this
interface.

Unexposed expressions have the same operations as any other kind
of expression; one can extract their location information,
spelling, children, etc. However, the specific kind of the
expression is not reported. */
  CXCursor_UnexposedExpr: 100,
  /** An expression that refers to some value declaration, such
as a function, variable, or enumerator. */
  CXCursor_DeclRefExpr: 101,
  /** An expression that refers to a member of a struct, union,
class, Objective-C class, etc. */
  CXCursor_MemberRefExpr: 102,
  /** An expression that calls a function. */
  CXCursor_CallExpr: 103,
  /** An expression that sends a message to an Objective-C
object or class. */
  CXCursor_ObjCMessageExpr: 104,
  /** An expression that represents a block literal. */
  CXCursor_BlockExpr: 105,
  /** An integer literal. */
  CXCursor_IntegerLiteral: 106,
  /** A floating point number literal. */
  CXCursor_FloatingLiteral: 107,
  /** An imaginary number literal. */
  CXCursor_ImaginaryLiteral: 108,
  /** A string literal. */
  CXCursor_StringLiteral: 109,
  /** A character literal. */
  CXCursor_CharacterLiteral: 110,
  /** A parenthesized expression, e.g. "(1)".

This AST node is only formed if full location information is requested. */
  CXCursor_ParenExpr: 111,
  /** This represents the unary-expression's (except sizeof and
alignof). */
  CXCursor_UnaryOperator: 112,
  /** [C99 6.5.2.1] Array Subscripting. */
  CXCursor_ArraySubscriptExpr: 113,
  /** A builtin binary operation expression such as "x + y" or
"x <= y". */
  CXCursor_BinaryOperator: 114,
  /** Compound assignment such as "+=". */
  CXCursor_CompoundAssignOperator: 115,
  /** The ?: ternary operator. */
  CXCursor_ConditionalOperator: 116,
  /** An explicit cast in C (C99 6.5.4) or a C-style cast in C++
(C++ [expr.cast]), which uses the syntax (Type)expr.

For example: (int)f. */
  CXCursor_CStyleCastExpr: 117,
  /** [C99 6.5.2.5] */
  CXCursor_CompoundLiteralExpr: 118,
  /** Describes an C or C++ initializer list. */
  CXCursor_InitListExpr: 119,
  /** The GNU address of label extension, representing &&label. */
  CXCursor_AddrLabelExpr: 120,
  /** This is the GNU Statement Expression extension: ({int X=4; X;}) */
  CXCursor_StmtExpr: 121,
  /** Represents a C11 generic selection. */
  CXCursor_GenericSelectionExpr: 122,
  /** Implements the GNU __null extension, which is a name for a null
pointer constant that has integral type (e.g., int or long) and is the same
size and alignment as a pointer.

The __null extension is typically only used by system headers, which define
NULL as __null in C++ rather than using 0 (which is an integer that may not
match the size of a pointer). */
  CXCursor_GNUNullExpr: 123,
  /** C++'s static_cast<> expression. */
  CXCursor_CXXStaticCastExpr: 124,
  /** C++'s dynamic_cast<> expression. */
  CXCursor_CXXDynamicCastExpr: 125,
  /** C++'s reinterpret_cast<> expression. */
  CXCursor_CXXReinterpretCastExpr: 126,
  /** C++'s const_cast<> expression. */
  CXCursor_CXXConstCastExpr: 127,
  /** Represents an explicit C++ type conversion that uses "functional"
notion (C++ [expr.type.conv]).

Example:
\code
x = int(0.5);
\endcode */
  CXCursor_CXXFunctionalCastExpr: 128,
  /** A C++ typeid expression (C++ [expr.typeid]). */
  CXCursor_CXXTypeidExpr: 129,
  /** [C++ 2.13.5] C++ Boolean Literal. */
  CXCursor_CXXBoolLiteralExpr: 130,
  /** [C++0x 2.14.7] C++ Pointer Literal. */
  CXCursor_CXXNullPtrLiteralExpr: 131,
  /** Represents the "this" expression in C++ */
  CXCursor_CXXThisExpr: 132,
  /** [C++ 15] C++ Throw Expression.

This handles 'throw' and 'throw' assignment-expression. When
assignment-expression isn't present, Op will be null. */
  CXCursor_CXXThrowExpr: 133,
  /** A new expression for memory allocation and constructor calls, e.g:
"new CXXNewExpr(foo)". */
  CXCursor_CXXNewExpr: 134,
  /** A delete expression for memory deallocation and destructor calls,
e.g. "delete[] pArray". */
  CXCursor_CXXDeleteExpr: 135,
  /** A unary expression. (noexcept, sizeof, or other traits) */
  CXCursor_UnaryExpr: 136,
  /** An Objective-C string literal i.e. @"foo". */
  CXCursor_ObjCStringLiteral: 137,
  /** An Objective-C \@encode expression. */
  CXCursor_ObjCEncodeExpr: 138,
  /** An Objective-C \@selector expression. */
  CXCursor_ObjCSelectorExpr: 139,
  /** An Objective-C \@protocol expression. */
  CXCursor_ObjCProtocolExpr: 140,
  /** An Objective-C "bridged" cast expression, which casts between
Objective-C pointers and C pointers, transferring ownership in the process.

\code
NSString *str = (__bridge_transfer NSString *)CFCreateString();
\endcode */
  CXCursor_ObjCBridgedCastExpr: 141,
  /** Represents a C++0x pack expansion that produces a sequence of
expressions.

A pack expansion expression contains a pattern (which itself is an
expression) followed by an ellipsis. For example:

\code
template<typename F, typename ...Types>
void forward(F f, Types &&...args) {
f(static_cast<Types&&>(args)...);
}
\endcode */
  CXCursor_PackExpansionExpr: 142,
  /** Represents an expression that computes the length of a parameter
pack.

\code
template<typename ...Types>
struct count {
static const unsigned value = sizeof...(Types);
};
\endcode */
  CXCursor_SizeOfPackExpr: 143,
  CXCursor_LambdaExpr: 144,
  /** Objective-c Boolean Literal. */
  CXCursor_ObjCBoolLiteralExpr: 145,
  /** Represents the "self" expression in an Objective-C method. */
  CXCursor_ObjCSelfExpr: 146,
  /** OpenMP 5.0 [2.1.5, Array Section].
OpenACC 3.3 [2.7.1, Data Specification for Data Clauses (Sub Arrays)] */
  CXCursor_ArraySectionExpr: 147,
  /** Represents an @available(...) check. */
  CXCursor_ObjCAvailabilityCheckExpr: 148,
  /** Fixed point literal */
  CXCursor_FixedPointLiteral: 149,
  /** OpenMP 5.0 [2.1.4, Array Shaping]. */
  CXCursor_OMPArrayShapingExpr: 150,
  /** OpenMP 5.0 [2.1.6 Iterators] */
  CXCursor_OMPIteratorExpr: 151,
  /** OpenCL's addrspace_cast<> expression. */
  CXCursor_CXXAddrspaceCastExpr: 152,
  /** Expression that references a C++20 concept. */
  CXCursor_ConceptSpecializationExpr: 153,
  /** Expression that references a C++20 requires expression. */
  CXCursor_RequiresExpr: 154,
  /** Expression that references a C++20 parenthesized list aggregate
initializer. */
  CXCursor_CXXParenListInitExpr: 155,
  /** Represents a C++26 pack indexing expression. */
  CXCursor_PackIndexingExpr: 156,
  /** Represents a C++26 pack indexing expression. */
  CXCursor_LastExpr: 156,
  /** Represents a C++26 pack indexing expression. */
  CXCursor_FirstStmt: 200,
  /** A statement whose specific kind is not exposed via this
interface.

Unexposed statements have the same operations as any other kind of
statement; one can extract their location information, spelling,
children, etc. However, the specific kind of the statement is not
reported. */
  CXCursor_UnexposedStmt: 200,
  /** A labelled statement in a function.

This cursor kind is used to describe the "start_over:" label statement in
the following example:

\code
start_over:
++counter;
\endcode */
  CXCursor_LabelStmt: 201,
  /** A group of statements like { stmt stmt }.

This cursor kind is used to describe compound statements, e.g. function
bodies. */
  CXCursor_CompoundStmt: 202,
  /** A case statement. */
  CXCursor_CaseStmt: 203,
  /** A default statement. */
  CXCursor_DefaultStmt: 204,
  /** An if statement */
  CXCursor_IfStmt: 205,
  /** A switch statement. */
  CXCursor_SwitchStmt: 206,
  /** A while statement. */
  CXCursor_WhileStmt: 207,
  /** A do statement. */
  CXCursor_DoStmt: 208,
  /** A for statement. */
  CXCursor_ForStmt: 209,
  /** A goto statement. */
  CXCursor_GotoStmt: 210,
  /** An indirect goto statement. */
  CXCursor_IndirectGotoStmt: 211,
  /** A continue statement. */
  CXCursor_ContinueStmt: 212,
  /** A break statement. */
  CXCursor_BreakStmt: 213,
  /** A return statement. */
  CXCursor_ReturnStmt: 214,
  /** A GCC inline assembly statement extension. */
  CXCursor_GCCAsmStmt: 215,
  /** A GCC inline assembly statement extension. */
  CXCursor_AsmStmt: 215,
  /** Objective-C's overall \@try-\@catch-\@finally statement. */
  CXCursor_ObjCAtTryStmt: 216,
  /** Objective-C's \@catch statement. */
  CXCursor_ObjCAtCatchStmt: 217,
  /** Objective-C's \@finally statement. */
  CXCursor_ObjCAtFinallyStmt: 218,
  /** Objective-C's \@throw statement. */
  CXCursor_ObjCAtThrowStmt: 219,
  /** Objective-C's \@synchronized statement. */
  CXCursor_ObjCAtSynchronizedStmt: 220,
  /** Objective-C's autorelease pool statement. */
  CXCursor_ObjCAutoreleasePoolStmt: 221,
  /** Objective-C's collection statement. */
  CXCursor_ObjCForCollectionStmt: 222,
  /** C++'s catch statement. */
  CXCursor_CXXCatchStmt: 223,
  /** C++'s try statement. */
  CXCursor_CXXTryStmt: 224,
  /** C++'s for (* : *) statement. */
  CXCursor_CXXForRangeStmt: 225,
  /** Windows Structured Exception Handling's try statement. */
  CXCursor_SEHTryStmt: 226,
  /** Windows Structured Exception Handling's except statement. */
  CXCursor_SEHExceptStmt: 227,
  /** Windows Structured Exception Handling's finally statement. */
  CXCursor_SEHFinallyStmt: 228,
  /** A MS inline assembly statement extension. */
  CXCursor_MSAsmStmt: 229,
  /** The null statement ";": C99 6.8.3p3.

This cursor kind is used to describe the null statement. */
  CXCursor_NullStmt: 230,
  /** Adaptor class for mixing declarations with statements and
expressions. */
  CXCursor_DeclStmt: 231,
  /** OpenMP parallel directive. */
  CXCursor_OMPParallelDirective: 232,
  /** OpenMP SIMD directive. */
  CXCursor_OMPSimdDirective: 233,
  /** OpenMP for directive. */
  CXCursor_OMPForDirective: 234,
  /** OpenMP sections directive. */
  CXCursor_OMPSectionsDirective: 235,
  /** OpenMP section directive. */
  CXCursor_OMPSectionDirective: 236,
  /** OpenMP single directive. */
  CXCursor_OMPSingleDirective: 237,
  /** OpenMP parallel for directive. */
  CXCursor_OMPParallelForDirective: 238,
  /** OpenMP parallel sections directive. */
  CXCursor_OMPParallelSectionsDirective: 239,
  /** OpenMP task directive. */
  CXCursor_OMPTaskDirective: 240,
  /** OpenMP master directive. */
  CXCursor_OMPMasterDirective: 241,
  /** OpenMP critical directive. */
  CXCursor_OMPCriticalDirective: 242,
  /** OpenMP taskyield directive. */
  CXCursor_OMPTaskyieldDirective: 243,
  /** OpenMP barrier directive. */
  CXCursor_OMPBarrierDirective: 244,
  /** OpenMP taskwait directive. */
  CXCursor_OMPTaskwaitDirective: 245,
  /** OpenMP flush directive. */
  CXCursor_OMPFlushDirective: 246,
  /** Windows Structured Exception Handling's leave statement. */
  CXCursor_SEHLeaveStmt: 247,
  /** OpenMP ordered directive. */
  CXCursor_OMPOrderedDirective: 248,
  /** OpenMP atomic directive. */
  CXCursor_OMPAtomicDirective: 249,
  /** OpenMP for SIMD directive. */
  CXCursor_OMPForSimdDirective: 250,
  /** OpenMP parallel for SIMD directive. */
  CXCursor_OMPParallelForSimdDirective: 251,
  /** OpenMP target directive. */
  CXCursor_OMPTargetDirective: 252,
  /** OpenMP teams directive. */
  CXCursor_OMPTeamsDirective: 253,
  /** OpenMP taskgroup directive. */
  CXCursor_OMPTaskgroupDirective: 254,
  /** OpenMP cancellation point directive. */
  CXCursor_OMPCancellationPointDirective: 255,
  /** OpenMP cancel directive. */
  CXCursor_OMPCancelDirective: 256,
  /** OpenMP target data directive. */
  CXCursor_OMPTargetDataDirective: 257,
  /** OpenMP taskloop directive. */
  CXCursor_OMPTaskLoopDirective: 258,
  /** OpenMP taskloop simd directive. */
  CXCursor_OMPTaskLoopSimdDirective: 259,
  /** OpenMP distribute directive. */
  CXCursor_OMPDistributeDirective: 260,
  /** OpenMP target enter data directive. */
  CXCursor_OMPTargetEnterDataDirective: 261,
  /** OpenMP target exit data directive. */
  CXCursor_OMPTargetExitDataDirective: 262,
  /** OpenMP target parallel directive. */
  CXCursor_OMPTargetParallelDirective: 263,
  /** OpenMP target parallel for directive. */
  CXCursor_OMPTargetParallelForDirective: 264,
  /** OpenMP target update directive. */
  CXCursor_OMPTargetUpdateDirective: 265,
  /** OpenMP distribute parallel for directive. */
  CXCursor_OMPDistributeParallelForDirective: 266,
  /** OpenMP distribute parallel for simd directive. */
  CXCursor_OMPDistributeParallelForSimdDirective: 267,
  /** OpenMP distribute simd directive. */
  CXCursor_OMPDistributeSimdDirective: 268,
  /** OpenMP target parallel for simd directive. */
  CXCursor_OMPTargetParallelForSimdDirective: 269,
  /** OpenMP target simd directive. */
  CXCursor_OMPTargetSimdDirective: 270,
  /** OpenMP teams distribute directive. */
  CXCursor_OMPTeamsDistributeDirective: 271,
  /** OpenMP teams distribute simd directive. */
  CXCursor_OMPTeamsDistributeSimdDirective: 272,
  /** OpenMP teams distribute parallel for simd directive. */
  CXCursor_OMPTeamsDistributeParallelForSimdDirective: 273,
  /** OpenMP teams distribute parallel for directive. */
  CXCursor_OMPTeamsDistributeParallelForDirective: 274,
  /** OpenMP target teams directive. */
  CXCursor_OMPTargetTeamsDirective: 275,
  /** OpenMP target teams distribute directive. */
  CXCursor_OMPTargetTeamsDistributeDirective: 276,
  /** OpenMP target teams distribute parallel for directive. */
  CXCursor_OMPTargetTeamsDistributeParallelForDirective: 277,
  /** OpenMP target teams distribute parallel for simd directive. */
  CXCursor_OMPTargetTeamsDistributeParallelForSimdDirective: 278,
  /** OpenMP target teams distribute simd directive. */
  CXCursor_OMPTargetTeamsDistributeSimdDirective: 279,
  /** C++2a std::bit_cast expression. */
  CXCursor_BuiltinBitCastExpr: 280,
  /** OpenMP master taskloop directive. */
  CXCursor_OMPMasterTaskLoopDirective: 281,
  /** OpenMP parallel master taskloop directive. */
  CXCursor_OMPParallelMasterTaskLoopDirective: 282,
  /** OpenMP master taskloop simd directive. */
  CXCursor_OMPMasterTaskLoopSimdDirective: 283,
  /** OpenMP parallel master taskloop simd directive. */
  CXCursor_OMPParallelMasterTaskLoopSimdDirective: 284,
  /** OpenMP parallel master directive. */
  CXCursor_OMPParallelMasterDirective: 285,
  /** OpenMP depobj directive. */
  CXCursor_OMPDepobjDirective: 286,
  /** OpenMP scan directive. */
  CXCursor_OMPScanDirective: 287,
  /** OpenMP tile directive. */
  CXCursor_OMPTileDirective: 288,
  /** OpenMP canonical loop. */
  CXCursor_OMPCanonicalLoop: 289,
  /** OpenMP interop directive. */
  CXCursor_OMPInteropDirective: 290,
  /** OpenMP dispatch directive. */
  CXCursor_OMPDispatchDirective: 291,
  /** OpenMP masked directive. */
  CXCursor_OMPMaskedDirective: 292,
  /** OpenMP unroll directive. */
  CXCursor_OMPUnrollDirective: 293,
  /** OpenMP metadirective directive. */
  CXCursor_OMPMetaDirective: 294,
  /** OpenMP loop directive. */
  CXCursor_OMPGenericLoopDirective: 295,
  /** OpenMP teams loop directive. */
  CXCursor_OMPTeamsGenericLoopDirective: 296,
  /** OpenMP target teams loop directive. */
  CXCursor_OMPTargetTeamsGenericLoopDirective: 297,
  /** OpenMP parallel loop directive. */
  CXCursor_OMPParallelGenericLoopDirective: 298,
  /** OpenMP target parallel loop directive. */
  CXCursor_OMPTargetParallelGenericLoopDirective: 299,
  /** OpenMP parallel masked directive. */
  CXCursor_OMPParallelMaskedDirective: 300,
  /** OpenMP masked taskloop directive. */
  CXCursor_OMPMaskedTaskLoopDirective: 301,
  /** OpenMP masked taskloop simd directive. */
  CXCursor_OMPMaskedTaskLoopSimdDirective: 302,
  /** OpenMP parallel masked taskloop directive. */
  CXCursor_OMPParallelMaskedTaskLoopDirective: 303,
  /** OpenMP parallel masked taskloop simd directive. */
  CXCursor_OMPParallelMaskedTaskLoopSimdDirective: 304,
  /** OpenMP error directive. */
  CXCursor_OMPErrorDirective: 305,
  /** OpenMP scope directive. */
  CXCursor_OMPScopeDirective: 306,
  /** OpenMP reverse directive. */
  CXCursor_OMPReverseDirective: 307,
  /** OpenMP interchange directive. */
  CXCursor_OMPInterchangeDirective: 308,
  /** OpenMP assume directive. */
  CXCursor_OMPAssumeDirective: 309,
  /** OpenACC Compute Construct. */
  CXCursor_OpenACCComputeConstruct: 320,
  /** OpenACC Loop Construct. */
  CXCursor_OpenACCLoopConstruct: 321,
  /** OpenACC Combined Constructs. */
  CXCursor_OpenACCCombinedConstruct: 322,
  /** OpenACC data Construct. */
  CXCursor_OpenACCDataConstruct: 323,
  /** OpenACC enter data Construct. */
  CXCursor_OpenACCEnterDataConstruct: 324,
  /** OpenACC exit data Construct. */
  CXCursor_OpenACCExitDataConstruct: 325,
  /** OpenACC host_data Construct. */
  CXCursor_OpenACCHostDataConstruct: 326,
  /** OpenACC wait Construct. */
  CXCursor_OpenACCWaitConstruct: 327,
  /** OpenACC init Construct. */
  CXCursor_OpenACCInitConstruct: 328,
  /** OpenACC shutdown Construct. */
  CXCursor_OpenACCShutdownConstruct: 329,
  /** OpenACC set Construct. */
  CXCursor_OpenACCSetConstruct: 330,
  /** OpenACC update Construct. */
  CXCursor_OpenACCUpdateConstruct: 331,
  /** OpenACC update Construct. */
  CXCursor_LastStmt: 331,
  /** Cursor that represents the translation unit itself.

The translation unit cursor exists primarily to act as the root
cursor for traversing the contents of a translation unit. */
  CXCursor_TranslationUnit: 350,
  /** Cursor that represents the translation unit itself.

The translation unit cursor exists primarily to act as the root
cursor for traversing the contents of a translation unit. */
  CXCursor_FirstAttr: 400,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_UnexposedAttr: 400,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_IBActionAttr: 401,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_IBOutletAttr: 402,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_IBOutletCollectionAttr: 403,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_CXXFinalAttr: 404,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_CXXOverrideAttr: 405,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_AnnotateAttr: 406,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_AsmLabelAttr: 407,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_PackedAttr: 408,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_PureAttr: 409,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_ConstAttr: 410,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_NoDuplicateAttr: 411,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_CUDAConstantAttr: 412,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_CUDADeviceAttr: 413,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_CUDAGlobalAttr: 414,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_CUDAHostAttr: 415,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_CUDASharedAttr: 416,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_VisibilityAttr: 417,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_DLLExport: 418,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_DLLImport: 419,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_NSReturnsRetained: 420,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_NSReturnsNotRetained: 421,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_NSReturnsAutoreleased: 422,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_NSConsumesSelf: 423,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_NSConsumed: 424,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_ObjCException: 425,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_ObjCNSObject: 426,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_ObjCIndependentClass: 427,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_ObjCPreciseLifetime: 428,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_ObjCReturnsInnerPointer: 429,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_ObjCRequiresSuper: 430,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_ObjCRootClass: 431,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_ObjCSubclassingRestricted: 432,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_ObjCExplicitProtocolImpl: 433,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_ObjCDesignatedInitializer: 434,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_ObjCRuntimeVisible: 435,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_ObjCBoxable: 436,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_FlagEnum: 437,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_ConvergentAttr: 438,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_WarnUnusedAttr: 439,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_WarnUnusedResultAttr: 440,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_AlignedAttr: 441,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_LastAttr: 441,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_PreprocessingDirective: 500,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_MacroDefinition: 501,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_MacroExpansion: 502,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_MacroInstantiation: 502,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_InclusionDirective: 503,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_FirstPreprocessing: 500,
  /** An attribute whose specific kind is not exposed via this
interface. */
  CXCursor_LastPreprocessing: 503,
  /** A module import declaration. */
  CXCursor_ModuleImportDecl: 600,
  /** A module import declaration. */
  CXCursor_TypeAliasTemplateDecl: 601,
  /** A static_assert or _Static_assert node */
  CXCursor_StaticAssert: 602,
  /** a friend declaration. */
  CXCursor_FriendDecl: 603,
  /** a concept declaration. */
  CXCursor_ConceptDecl: 604,
  /** a concept declaration. */
  CXCursor_FirstExtraDecl: 600,
  /** a concept declaration. */
  CXCursor_LastExtraDecl: 604,
  /** A code completion overload candidate. */
  CXCursor_OverloadCandidate: 700,
} as const;

/** A declaration whose specific kind is not exposed via this
interface.

Unexposed declarations have the same operations as any other kind
of declaration; one can extract their location information,
spelling, find their definitions, etc. However, the specific kind
of the declaration is not reported. */
export const CXCursor_UnexposedDecl: number = CXCursorKind.CXCursor_UnexposedDecl;
/** A C or C++ struct. */
export const CXCursor_StructDecl: number = CXCursorKind.CXCursor_StructDecl;
/** A C or C++ union. */
export const CXCursor_UnionDecl: number = CXCursorKind.CXCursor_UnionDecl;
/** A C++ class. */
export const CXCursor_ClassDecl: number = CXCursorKind.CXCursor_ClassDecl;
/** An enumeration. */
export const CXCursor_EnumDecl: number = CXCursorKind.CXCursor_EnumDecl;
/** A field (in C) or non-static data member (in C++) in a
struct, union, or C++ class. */
export const CXCursor_FieldDecl: number = CXCursorKind.CXCursor_FieldDecl;
/** An enumerator constant. */
export const CXCursor_EnumConstantDecl: number = CXCursorKind.CXCursor_EnumConstantDecl;
/** A function. */
export const CXCursor_FunctionDecl: number = CXCursorKind.CXCursor_FunctionDecl;
/** A variable. */
export const CXCursor_VarDecl: number = CXCursorKind.CXCursor_VarDecl;
/** A function or method parameter. */
export const CXCursor_ParmDecl: number = CXCursorKind.CXCursor_ParmDecl;
/** An Objective-C \@interface. */
export const CXCursor_ObjCInterfaceDecl: number = CXCursorKind.CXCursor_ObjCInterfaceDecl;
/** An Objective-C \@interface for a category. */
export const CXCursor_ObjCCategoryDecl: number = CXCursorKind.CXCursor_ObjCCategoryDecl;
/** An Objective-C \@protocol declaration. */
export const CXCursor_ObjCProtocolDecl: number = CXCursorKind.CXCursor_ObjCProtocolDecl;
/** An Objective-C \@property declaration. */
export const CXCursor_ObjCPropertyDecl: number = CXCursorKind.CXCursor_ObjCPropertyDecl;
/** An Objective-C instance variable. */
export const CXCursor_ObjCIvarDecl: number = CXCursorKind.CXCursor_ObjCIvarDecl;
/** An Objective-C instance method. */
export const CXCursor_ObjCInstanceMethodDecl: number = CXCursorKind.CXCursor_ObjCInstanceMethodDecl;
/** An Objective-C class method. */
export const CXCursor_ObjCClassMethodDecl: number = CXCursorKind.CXCursor_ObjCClassMethodDecl;
/** An Objective-C \@implementation. */
export const CXCursor_ObjCImplementationDecl: number = CXCursorKind.CXCursor_ObjCImplementationDecl;
/** An Objective-C \@implementation for a category. */
export const CXCursor_ObjCCategoryImplDecl: number = CXCursorKind.CXCursor_ObjCCategoryImplDecl;
/** A typedef. */
export const CXCursor_TypedefDecl: number = CXCursorKind.CXCursor_TypedefDecl;
/** A C++ class method. */
export const CXCursor_CXXMethod: number = CXCursorKind.CXCursor_CXXMethod;
/** A C++ namespace. */
export const CXCursor_Namespace: number = CXCursorKind.CXCursor_Namespace;
/** A linkage specification, e.g. 'extern "C"'. */
export const CXCursor_LinkageSpec: number = CXCursorKind.CXCursor_LinkageSpec;
/** A C++ constructor. */
export const CXCursor_Constructor: number = CXCursorKind.CXCursor_Constructor;
/** A C++ destructor. */
export const CXCursor_Destructor: number = CXCursorKind.CXCursor_Destructor;
/** A C++ conversion function. */
export const CXCursor_ConversionFunction: number = CXCursorKind.CXCursor_ConversionFunction;
/** A C++ template type parameter. */
export const CXCursor_TemplateTypeParameter: number = CXCursorKind.CXCursor_TemplateTypeParameter;
/** A C++ non-type template parameter. */
export const CXCursor_NonTypeTemplateParameter: number = CXCursorKind.CXCursor_NonTypeTemplateParameter;
/** A C++ template template parameter. */
export const CXCursor_TemplateTemplateParameter: number = CXCursorKind.CXCursor_TemplateTemplateParameter;
/** A C++ function template. */
export const CXCursor_FunctionTemplate: number = CXCursorKind.CXCursor_FunctionTemplate;
/** A C++ class template. */
export const CXCursor_ClassTemplate: number = CXCursorKind.CXCursor_ClassTemplate;
/** A C++ class template partial specialization. */
export const CXCursor_ClassTemplatePartialSpecialization: number = CXCursorKind.CXCursor_ClassTemplatePartialSpecialization;
/** A C++ namespace alias declaration. */
export const CXCursor_NamespaceAlias: number = CXCursorKind.CXCursor_NamespaceAlias;
/** A C++ using directive. */
export const CXCursor_UsingDirective: number = CXCursorKind.CXCursor_UsingDirective;
/** A C++ using declaration. */
export const CXCursor_UsingDeclaration: number = CXCursorKind.CXCursor_UsingDeclaration;
/** A C++ alias declaration */
export const CXCursor_TypeAliasDecl: number = CXCursorKind.CXCursor_TypeAliasDecl;
/** An Objective-C \@synthesize definition. */
export const CXCursor_ObjCSynthesizeDecl: number = CXCursorKind.CXCursor_ObjCSynthesizeDecl;
/** An Objective-C \@dynamic definition. */
export const CXCursor_ObjCDynamicDecl: number = CXCursorKind.CXCursor_ObjCDynamicDecl;
/** An access specifier. */
export const CXCursor_CXXAccessSpecifier: number = CXCursorKind.CXCursor_CXXAccessSpecifier;
/** An access specifier. */
export const CXCursor_FirstDecl: number = CXCursorKind.CXCursor_FirstDecl;
/** An access specifier. */
export const CXCursor_LastDecl: number = CXCursorKind.CXCursor_LastDecl;
/** An access specifier. */
export const CXCursor_FirstRef: number = CXCursorKind.CXCursor_FirstRef;
/** An access specifier. */
export const CXCursor_ObjCSuperClassRef: number = CXCursorKind.CXCursor_ObjCSuperClassRef;
/** An access specifier. */
export const CXCursor_ObjCProtocolRef: number = CXCursorKind.CXCursor_ObjCProtocolRef;
/** An access specifier. */
export const CXCursor_ObjCClassRef: number = CXCursorKind.CXCursor_ObjCClassRef;
/** A reference to a type declaration.

A type reference occurs anywhere where a type is named but not
declared. For example, given:

\code
typedef unsigned size_type;
size_type size;
\endcode

The typedef is a declaration of size_type (CXCursor_TypedefDecl),
while the type of the variable "size" is referenced. The cursor
referenced by the type of size is the typedef for size_type. */
export const CXCursor_TypeRef: number = CXCursorKind.CXCursor_TypeRef;
/** A reference to a type declaration.

A type reference occurs anywhere where a type is named but not
declared. For example, given:

\code
typedef unsigned size_type;
size_type size;
\endcode

The typedef is a declaration of size_type (CXCursor_TypedefDecl),
while the type of the variable "size" is referenced. The cursor
referenced by the type of size is the typedef for size_type. */
export const CXCursor_CXXBaseSpecifier: number = CXCursorKind.CXCursor_CXXBaseSpecifier;
/** A reference to a class template, function template, template
template parameter, or class template partial specialization. */
export const CXCursor_TemplateRef: number = CXCursorKind.CXCursor_TemplateRef;
/** A reference to a namespace or namespace alias. */
export const CXCursor_NamespaceRef: number = CXCursorKind.CXCursor_NamespaceRef;
/** A reference to a member of a struct, union, or class that occurs in
some non-expression context, e.g., a designated initializer. */
export const CXCursor_MemberRef: number = CXCursorKind.CXCursor_MemberRef;
/** A reference to a labeled statement.

This cursor kind is used to describe the jump to "start_over" in the
goto statement in the following example:

\code
start_over:
++counter;

goto start_over;
\endcode

A label reference cursor refers to a label statement. */
export const CXCursor_LabelRef: number = CXCursorKind.CXCursor_LabelRef;
/** A reference to a set of overloaded functions or function templates
that has not yet been resolved to a specific function or function template.

An overloaded declaration reference cursor occurs in C++ templates where
a dependent name refers to a function. For example:

\code
template<typename T> void swap(T&, T&);

struct X { ... };
void swap(X&, X&);

template<typename T>
void reverse(T* first, T* last) {
while (first < last - 1) {
swap(*first, *--last);
++first;
}
}

struct Y { };
void swap(Y&, Y&);
\endcode

Here, the identifier "swap" is associated with an overloaded declaration
reference. In the template definition, "swap" refers to either of the two
"swap" functions declared above, so both results will be available. At
instantiation time, "swap" may also refer to other functions found via
argument-dependent lookup (e.g., the "swap" function at the end of the
example).

The functions \c clang_getNumOverloadedDecls() and
\c clang_getOverloadedDecl() can be used to retrieve the definitions
referenced by this cursor. */
export const CXCursor_OverloadedDeclRef: number = CXCursorKind.CXCursor_OverloadedDeclRef;
/** A reference to a variable that occurs in some non-expression
context, e.g., a C++ lambda capture list. */
export const CXCursor_VariableRef: number = CXCursorKind.CXCursor_VariableRef;
/** A reference to a variable that occurs in some non-expression
context, e.g., a C++ lambda capture list. */
export const CXCursor_LastRef: number = CXCursorKind.CXCursor_LastRef;
/** A reference to a variable that occurs in some non-expression
context, e.g., a C++ lambda capture list. */
export const CXCursor_FirstInvalid: number = CXCursorKind.CXCursor_FirstInvalid;
/** A reference to a variable that occurs in some non-expression
context, e.g., a C++ lambda capture list. */
export const CXCursor_InvalidFile: number = CXCursorKind.CXCursor_InvalidFile;
/** A reference to a variable that occurs in some non-expression
context, e.g., a C++ lambda capture list. */
export const CXCursor_NoDeclFound: number = CXCursorKind.CXCursor_NoDeclFound;
/** A reference to a variable that occurs in some non-expression
context, e.g., a C++ lambda capture list. */
export const CXCursor_NotImplemented: number = CXCursorKind.CXCursor_NotImplemented;
/** A reference to a variable that occurs in some non-expression
context, e.g., a C++ lambda capture list. */
export const CXCursor_InvalidCode: number = CXCursorKind.CXCursor_InvalidCode;
/** A reference to a variable that occurs in some non-expression
context, e.g., a C++ lambda capture list. */
export const CXCursor_LastInvalid: number = CXCursorKind.CXCursor_LastInvalid;
/** A reference to a variable that occurs in some non-expression
context, e.g., a C++ lambda capture list. */
export const CXCursor_FirstExpr: number = CXCursorKind.CXCursor_FirstExpr;
/** An expression whose specific kind is not exposed via this
interface.

Unexposed expressions have the same operations as any other kind
of expression; one can extract their location information,
spelling, children, etc. However, the specific kind of the
expression is not reported. */
export const CXCursor_UnexposedExpr: number = CXCursorKind.CXCursor_UnexposedExpr;
/** An expression that refers to some value declaration, such
as a function, variable, or enumerator. */
export const CXCursor_DeclRefExpr: number = CXCursorKind.CXCursor_DeclRefExpr;
/** An expression that refers to a member of a struct, union,
class, Objective-C class, etc. */
export const CXCursor_MemberRefExpr: number = CXCursorKind.CXCursor_MemberRefExpr;
/** An expression that calls a function. */
export const CXCursor_CallExpr: number = CXCursorKind.CXCursor_CallExpr;
/** An expression that sends a message to an Objective-C
object or class. */
export const CXCursor_ObjCMessageExpr: number = CXCursorKind.CXCursor_ObjCMessageExpr;
/** An expression that represents a block literal. */
export const CXCursor_BlockExpr: number = CXCursorKind.CXCursor_BlockExpr;
/** An integer literal. */
export const CXCursor_IntegerLiteral: number = CXCursorKind.CXCursor_IntegerLiteral;
/** A floating point number literal. */
export const CXCursor_FloatingLiteral: number = CXCursorKind.CXCursor_FloatingLiteral;
/** An imaginary number literal. */
export const CXCursor_ImaginaryLiteral: number = CXCursorKind.CXCursor_ImaginaryLiteral;
/** A string literal. */
export const CXCursor_StringLiteral: number = CXCursorKind.CXCursor_StringLiteral;
/** A character literal. */
export const CXCursor_CharacterLiteral: number = CXCursorKind.CXCursor_CharacterLiteral;
/** A parenthesized expression, e.g. "(1)".

This AST node is only formed if full location information is requested. */
export const CXCursor_ParenExpr: number = CXCursorKind.CXCursor_ParenExpr;
/** This represents the unary-expression's (except sizeof and
alignof). */
export const CXCursor_UnaryOperator: number = CXCursorKind.CXCursor_UnaryOperator;
/** [C99 6.5.2.1] Array Subscripting. */
export const CXCursor_ArraySubscriptExpr: number = CXCursorKind.CXCursor_ArraySubscriptExpr;
/** A builtin binary operation expression such as "x + y" or
"x <= y". */
export const CXCursor_BinaryOperator: number = CXCursorKind.CXCursor_BinaryOperator;
/** Compound assignment such as "+=". */
export const CXCursor_CompoundAssignOperator: number = CXCursorKind.CXCursor_CompoundAssignOperator;
/** The ?: ternary operator. */
export const CXCursor_ConditionalOperator: number = CXCursorKind.CXCursor_ConditionalOperator;
/** An explicit cast in C (C99 6.5.4) or a C-style cast in C++
(C++ [expr.cast]), which uses the syntax (Type)expr.

For example: (int)f. */
export const CXCursor_CStyleCastExpr: number = CXCursorKind.CXCursor_CStyleCastExpr;
/** [C99 6.5.2.5] */
export const CXCursor_CompoundLiteralExpr: number = CXCursorKind.CXCursor_CompoundLiteralExpr;
/** Describes an C or C++ initializer list. */
export const CXCursor_InitListExpr: number = CXCursorKind.CXCursor_InitListExpr;
/** The GNU address of label extension, representing &&label. */
export const CXCursor_AddrLabelExpr: number = CXCursorKind.CXCursor_AddrLabelExpr;
/** This is the GNU Statement Expression extension: ({int X=4; X;}) */
export const CXCursor_StmtExpr: number = CXCursorKind.CXCursor_StmtExpr;
/** Represents a C11 generic selection. */
export const CXCursor_GenericSelectionExpr: number = CXCursorKind.CXCursor_GenericSelectionExpr;
/** Implements the GNU __null extension, which is a name for a null
pointer constant that has integral type (e.g., int or long) and is the same
size and alignment as a pointer.

The __null extension is typically only used by system headers, which define
NULL as __null in C++ rather than using 0 (which is an integer that may not
match the size of a pointer). */
export const CXCursor_GNUNullExpr: number = CXCursorKind.CXCursor_GNUNullExpr;
/** C++'s static_cast<> expression. */
export const CXCursor_CXXStaticCastExpr: number = CXCursorKind.CXCursor_CXXStaticCastExpr;
/** C++'s dynamic_cast<> expression. */
export const CXCursor_CXXDynamicCastExpr: number = CXCursorKind.CXCursor_CXXDynamicCastExpr;
/** C++'s reinterpret_cast<> expression. */
export const CXCursor_CXXReinterpretCastExpr: number = CXCursorKind.CXCursor_CXXReinterpretCastExpr;
/** C++'s const_cast<> expression. */
export const CXCursor_CXXConstCastExpr: number = CXCursorKind.CXCursor_CXXConstCastExpr;
/** Represents an explicit C++ type conversion that uses "functional"
notion (C++ [expr.type.conv]).

Example:
\code
x = int(0.5);
\endcode */
export const CXCursor_CXXFunctionalCastExpr: number = CXCursorKind.CXCursor_CXXFunctionalCastExpr;
/** A C++ typeid expression (C++ [expr.typeid]). */
export const CXCursor_CXXTypeidExpr: number = CXCursorKind.CXCursor_CXXTypeidExpr;
/** [C++ 2.13.5] C++ Boolean Literal. */
export const CXCursor_CXXBoolLiteralExpr: number = CXCursorKind.CXCursor_CXXBoolLiteralExpr;
/** [C++0x 2.14.7] C++ Pointer Literal. */
export const CXCursor_CXXNullPtrLiteralExpr: number = CXCursorKind.CXCursor_CXXNullPtrLiteralExpr;
/** Represents the "this" expression in C++ */
export const CXCursor_CXXThisExpr: number = CXCursorKind.CXCursor_CXXThisExpr;
/** [C++ 15] C++ Throw Expression.

This handles 'throw' and 'throw' assignment-expression. When
assignment-expression isn't present, Op will be null. */
export const CXCursor_CXXThrowExpr: number = CXCursorKind.CXCursor_CXXThrowExpr;
/** A new expression for memory allocation and constructor calls, e.g:
"new CXXNewExpr(foo)". */
export const CXCursor_CXXNewExpr: number = CXCursorKind.CXCursor_CXXNewExpr;
/** A delete expression for memory deallocation and destructor calls,
e.g. "delete[] pArray". */
export const CXCursor_CXXDeleteExpr: number = CXCursorKind.CXCursor_CXXDeleteExpr;
/** A unary expression. (noexcept, sizeof, or other traits) */
export const CXCursor_UnaryExpr: number = CXCursorKind.CXCursor_UnaryExpr;
/** An Objective-C string literal i.e. @"foo". */
export const CXCursor_ObjCStringLiteral: number = CXCursorKind.CXCursor_ObjCStringLiteral;
/** An Objective-C \@encode expression. */
export const CXCursor_ObjCEncodeExpr: number = CXCursorKind.CXCursor_ObjCEncodeExpr;
/** An Objective-C \@selector expression. */
export const CXCursor_ObjCSelectorExpr: number = CXCursorKind.CXCursor_ObjCSelectorExpr;
/** An Objective-C \@protocol expression. */
export const CXCursor_ObjCProtocolExpr: number = CXCursorKind.CXCursor_ObjCProtocolExpr;
/** An Objective-C "bridged" cast expression, which casts between
Objective-C pointers and C pointers, transferring ownership in the process.

\code
NSString *str = (__bridge_transfer NSString *)CFCreateString();
\endcode */
export const CXCursor_ObjCBridgedCastExpr: number = CXCursorKind.CXCursor_ObjCBridgedCastExpr;
/** Represents a C++0x pack expansion that produces a sequence of
expressions.

A pack expansion expression contains a pattern (which itself is an
expression) followed by an ellipsis. For example:

\code
template<typename F, typename ...Types>
void forward(F f, Types &&...args) {
f(static_cast<Types&&>(args)...);
}
\endcode */
export const CXCursor_PackExpansionExpr: number = CXCursorKind.CXCursor_PackExpansionExpr;
/** Represents an expression that computes the length of a parameter
pack.

\code
template<typename ...Types>
struct count {
static const unsigned value = sizeof...(Types);
};
\endcode */
export const CXCursor_SizeOfPackExpr: number = CXCursorKind.CXCursor_SizeOfPackExpr;
export const CXCursor_LambdaExpr: number = CXCursorKind.CXCursor_LambdaExpr;
/** Objective-c Boolean Literal. */
export const CXCursor_ObjCBoolLiteralExpr: number = CXCursorKind.CXCursor_ObjCBoolLiteralExpr;
/** Represents the "self" expression in an Objective-C method. */
export const CXCursor_ObjCSelfExpr: number = CXCursorKind.CXCursor_ObjCSelfExpr;
/** OpenMP 5.0 [2.1.5, Array Section].
OpenACC 3.3 [2.7.1, Data Specification for Data Clauses (Sub Arrays)] */
export const CXCursor_ArraySectionExpr: number = CXCursorKind.CXCursor_ArraySectionExpr;
/** Represents an @available(...) check. */
export const CXCursor_ObjCAvailabilityCheckExpr: number = CXCursorKind.CXCursor_ObjCAvailabilityCheckExpr;
/** Fixed point literal */
export const CXCursor_FixedPointLiteral: number = CXCursorKind.CXCursor_FixedPointLiteral;
/** OpenMP 5.0 [2.1.4, Array Shaping]. */
export const CXCursor_OMPArrayShapingExpr: number = CXCursorKind.CXCursor_OMPArrayShapingExpr;
/** OpenMP 5.0 [2.1.6 Iterators] */
export const CXCursor_OMPIteratorExpr: number = CXCursorKind.CXCursor_OMPIteratorExpr;
/** OpenCL's addrspace_cast<> expression. */
export const CXCursor_CXXAddrspaceCastExpr: number = CXCursorKind.CXCursor_CXXAddrspaceCastExpr;
/** Expression that references a C++20 concept. */
export const CXCursor_ConceptSpecializationExpr: number = CXCursorKind.CXCursor_ConceptSpecializationExpr;
/** Expression that references a C++20 requires expression. */
export const CXCursor_RequiresExpr: number = CXCursorKind.CXCursor_RequiresExpr;
/** Expression that references a C++20 parenthesized list aggregate
initializer. */
export const CXCursor_CXXParenListInitExpr: number = CXCursorKind.CXCursor_CXXParenListInitExpr;
/** Represents a C++26 pack indexing expression. */
export const CXCursor_PackIndexingExpr: number = CXCursorKind.CXCursor_PackIndexingExpr;
/** Represents a C++26 pack indexing expression. */
export const CXCursor_LastExpr: number = CXCursorKind.CXCursor_LastExpr;
/** Represents a C++26 pack indexing expression. */
export const CXCursor_FirstStmt: number = CXCursorKind.CXCursor_FirstStmt;
/** A statement whose specific kind is not exposed via this
interface.

Unexposed statements have the same operations as any other kind of
statement; one can extract their location information, spelling,
children, etc. However, the specific kind of the statement is not
reported. */
export const CXCursor_UnexposedStmt: number = CXCursorKind.CXCursor_UnexposedStmt;
/** A labelled statement in a function.

This cursor kind is used to describe the "start_over:" label statement in
the following example:

\code
start_over:
++counter;
\endcode */
export const CXCursor_LabelStmt: number = CXCursorKind.CXCursor_LabelStmt;
/** A group of statements like { stmt stmt }.

This cursor kind is used to describe compound statements, e.g. function
bodies. */
export const CXCursor_CompoundStmt: number = CXCursorKind.CXCursor_CompoundStmt;
/** A case statement. */
export const CXCursor_CaseStmt: number = CXCursorKind.CXCursor_CaseStmt;
/** A default statement. */
export const CXCursor_DefaultStmt: number = CXCursorKind.CXCursor_DefaultStmt;
/** An if statement */
export const CXCursor_IfStmt: number = CXCursorKind.CXCursor_IfStmt;
/** A switch statement. */
export const CXCursor_SwitchStmt: number = CXCursorKind.CXCursor_SwitchStmt;
/** A while statement. */
export const CXCursor_WhileStmt: number = CXCursorKind.CXCursor_WhileStmt;
/** A do statement. */
export const CXCursor_DoStmt: number = CXCursorKind.CXCursor_DoStmt;
/** A for statement. */
export const CXCursor_ForStmt: number = CXCursorKind.CXCursor_ForStmt;
/** A goto statement. */
export const CXCursor_GotoStmt: number = CXCursorKind.CXCursor_GotoStmt;
/** An indirect goto statement. */
export const CXCursor_IndirectGotoStmt: number = CXCursorKind.CXCursor_IndirectGotoStmt;
/** A continue statement. */
export const CXCursor_ContinueStmt: number = CXCursorKind.CXCursor_ContinueStmt;
/** A break statement. */
export const CXCursor_BreakStmt: number = CXCursorKind.CXCursor_BreakStmt;
/** A return statement. */
export const CXCursor_ReturnStmt: number = CXCursorKind.CXCursor_ReturnStmt;
/** A GCC inline assembly statement extension. */
export const CXCursor_GCCAsmStmt: number = CXCursorKind.CXCursor_GCCAsmStmt;
/** A GCC inline assembly statement extension. */
export const CXCursor_AsmStmt: number = CXCursorKind.CXCursor_AsmStmt;
/** Objective-C's overall \@try-\@catch-\@finally statement. */
export const CXCursor_ObjCAtTryStmt: number = CXCursorKind.CXCursor_ObjCAtTryStmt;
/** Objective-C's \@catch statement. */
export const CXCursor_ObjCAtCatchStmt: number = CXCursorKind.CXCursor_ObjCAtCatchStmt;
/** Objective-C's \@finally statement. */
export const CXCursor_ObjCAtFinallyStmt: number = CXCursorKind.CXCursor_ObjCAtFinallyStmt;
/** Objective-C's \@throw statement. */
export const CXCursor_ObjCAtThrowStmt: number = CXCursorKind.CXCursor_ObjCAtThrowStmt;
/** Objective-C's \@synchronized statement. */
export const CXCursor_ObjCAtSynchronizedStmt: number = CXCursorKind.CXCursor_ObjCAtSynchronizedStmt;
/** Objective-C's autorelease pool statement. */
export const CXCursor_ObjCAutoreleasePoolStmt: number = CXCursorKind.CXCursor_ObjCAutoreleasePoolStmt;
/** Objective-C's collection statement. */
export const CXCursor_ObjCForCollectionStmt: number = CXCursorKind.CXCursor_ObjCForCollectionStmt;
/** C++'s catch statement. */
export const CXCursor_CXXCatchStmt: number = CXCursorKind.CXCursor_CXXCatchStmt;
/** C++'s try statement. */
export const CXCursor_CXXTryStmt: number = CXCursorKind.CXCursor_CXXTryStmt;
/** C++'s for (* : *) statement. */
export const CXCursor_CXXForRangeStmt: number = CXCursorKind.CXCursor_CXXForRangeStmt;
/** Windows Structured Exception Handling's try statement. */
export const CXCursor_SEHTryStmt: number = CXCursorKind.CXCursor_SEHTryStmt;
/** Windows Structured Exception Handling's except statement. */
export const CXCursor_SEHExceptStmt: number = CXCursorKind.CXCursor_SEHExceptStmt;
/** Windows Structured Exception Handling's finally statement. */
export const CXCursor_SEHFinallyStmt: number = CXCursorKind.CXCursor_SEHFinallyStmt;
/** A MS inline assembly statement extension. */
export const CXCursor_MSAsmStmt: number = CXCursorKind.CXCursor_MSAsmStmt;
/** The null statement ";": C99 6.8.3p3.

This cursor kind is used to describe the null statement. */
export const CXCursor_NullStmt: number = CXCursorKind.CXCursor_NullStmt;
/** Adaptor class for mixing declarations with statements and
expressions. */
export const CXCursor_DeclStmt: number = CXCursorKind.CXCursor_DeclStmt;
/** OpenMP parallel directive. */
export const CXCursor_OMPParallelDirective: number = CXCursorKind.CXCursor_OMPParallelDirective;
/** OpenMP SIMD directive. */
export const CXCursor_OMPSimdDirective: number = CXCursorKind.CXCursor_OMPSimdDirective;
/** OpenMP for directive. */
export const CXCursor_OMPForDirective: number = CXCursorKind.CXCursor_OMPForDirective;
/** OpenMP sections directive. */
export const CXCursor_OMPSectionsDirective: number = CXCursorKind.CXCursor_OMPSectionsDirective;
/** OpenMP section directive. */
export const CXCursor_OMPSectionDirective: number = CXCursorKind.CXCursor_OMPSectionDirective;
/** OpenMP single directive. */
export const CXCursor_OMPSingleDirective: number = CXCursorKind.CXCursor_OMPSingleDirective;
/** OpenMP parallel for directive. */
export const CXCursor_OMPParallelForDirective: number = CXCursorKind.CXCursor_OMPParallelForDirective;
/** OpenMP parallel sections directive. */
export const CXCursor_OMPParallelSectionsDirective: number = CXCursorKind.CXCursor_OMPParallelSectionsDirective;
/** OpenMP task directive. */
export const CXCursor_OMPTaskDirective: number = CXCursorKind.CXCursor_OMPTaskDirective;
/** OpenMP master directive. */
export const CXCursor_OMPMasterDirective: number = CXCursorKind.CXCursor_OMPMasterDirective;
/** OpenMP critical directive. */
export const CXCursor_OMPCriticalDirective: number = CXCursorKind.CXCursor_OMPCriticalDirective;
/** OpenMP taskyield directive. */
export const CXCursor_OMPTaskyieldDirective: number = CXCursorKind.CXCursor_OMPTaskyieldDirective;
/** OpenMP barrier directive. */
export const CXCursor_OMPBarrierDirective: number = CXCursorKind.CXCursor_OMPBarrierDirective;
/** OpenMP taskwait directive. */
export const CXCursor_OMPTaskwaitDirective: number = CXCursorKind.CXCursor_OMPTaskwaitDirective;
/** OpenMP flush directive. */
export const CXCursor_OMPFlushDirective: number = CXCursorKind.CXCursor_OMPFlushDirective;
/** Windows Structured Exception Handling's leave statement. */
export const CXCursor_SEHLeaveStmt: number = CXCursorKind.CXCursor_SEHLeaveStmt;
/** OpenMP ordered directive. */
export const CXCursor_OMPOrderedDirective: number = CXCursorKind.CXCursor_OMPOrderedDirective;
/** OpenMP atomic directive. */
export const CXCursor_OMPAtomicDirective: number = CXCursorKind.CXCursor_OMPAtomicDirective;
/** OpenMP for SIMD directive. */
export const CXCursor_OMPForSimdDirective: number = CXCursorKind.CXCursor_OMPForSimdDirective;
/** OpenMP parallel for SIMD directive. */
export const CXCursor_OMPParallelForSimdDirective: number = CXCursorKind.CXCursor_OMPParallelForSimdDirective;
/** OpenMP target directive. */
export const CXCursor_OMPTargetDirective: number = CXCursorKind.CXCursor_OMPTargetDirective;
/** OpenMP teams directive. */
export const CXCursor_OMPTeamsDirective: number = CXCursorKind.CXCursor_OMPTeamsDirective;
/** OpenMP taskgroup directive. */
export const CXCursor_OMPTaskgroupDirective: number = CXCursorKind.CXCursor_OMPTaskgroupDirective;
/** OpenMP cancellation point directive. */
export const CXCursor_OMPCancellationPointDirective: number = CXCursorKind.CXCursor_OMPCancellationPointDirective;
/** OpenMP cancel directive. */
export const CXCursor_OMPCancelDirective: number = CXCursorKind.CXCursor_OMPCancelDirective;
/** OpenMP target data directive. */
export const CXCursor_OMPTargetDataDirective: number = CXCursorKind.CXCursor_OMPTargetDataDirective;
/** OpenMP taskloop directive. */
export const CXCursor_OMPTaskLoopDirective: number = CXCursorKind.CXCursor_OMPTaskLoopDirective;
/** OpenMP taskloop simd directive. */
export const CXCursor_OMPTaskLoopSimdDirective: number = CXCursorKind.CXCursor_OMPTaskLoopSimdDirective;
/** OpenMP distribute directive. */
export const CXCursor_OMPDistributeDirective: number = CXCursorKind.CXCursor_OMPDistributeDirective;
/** OpenMP target enter data directive. */
export const CXCursor_OMPTargetEnterDataDirective: number = CXCursorKind.CXCursor_OMPTargetEnterDataDirective;
/** OpenMP target exit data directive. */
export const CXCursor_OMPTargetExitDataDirective: number = CXCursorKind.CXCursor_OMPTargetExitDataDirective;
/** OpenMP target parallel directive. */
export const CXCursor_OMPTargetParallelDirective: number = CXCursorKind.CXCursor_OMPTargetParallelDirective;
/** OpenMP target parallel for directive. */
export const CXCursor_OMPTargetParallelForDirective: number = CXCursorKind.CXCursor_OMPTargetParallelForDirective;
/** OpenMP target update directive. */
export const CXCursor_OMPTargetUpdateDirective: number = CXCursorKind.CXCursor_OMPTargetUpdateDirective;
/** OpenMP distribute parallel for directive. */
export const CXCursor_OMPDistributeParallelForDirective: number = CXCursorKind.CXCursor_OMPDistributeParallelForDirective;
/** OpenMP distribute parallel for simd directive. */
export const CXCursor_OMPDistributeParallelForSimdDirective: number = CXCursorKind.CXCursor_OMPDistributeParallelForSimdDirective;
/** OpenMP distribute simd directive. */
export const CXCursor_OMPDistributeSimdDirective: number = CXCursorKind.CXCursor_OMPDistributeSimdDirective;
/** OpenMP target parallel for simd directive. */
export const CXCursor_OMPTargetParallelForSimdDirective: number = CXCursorKind.CXCursor_OMPTargetParallelForSimdDirective;
/** OpenMP target simd directive. */
export const CXCursor_OMPTargetSimdDirective: number = CXCursorKind.CXCursor_OMPTargetSimdDirective;
/** OpenMP teams distribute directive. */
export const CXCursor_OMPTeamsDistributeDirective: number = CXCursorKind.CXCursor_OMPTeamsDistributeDirective;
/** OpenMP teams distribute simd directive. */
export const CXCursor_OMPTeamsDistributeSimdDirective: number = CXCursorKind.CXCursor_OMPTeamsDistributeSimdDirective;
/** OpenMP teams distribute parallel for simd directive. */
export const CXCursor_OMPTeamsDistributeParallelForSimdDirective: number = CXCursorKind.CXCursor_OMPTeamsDistributeParallelForSimdDirective;
/** OpenMP teams distribute parallel for directive. */
export const CXCursor_OMPTeamsDistributeParallelForDirective: number = CXCursorKind.CXCursor_OMPTeamsDistributeParallelForDirective;
/** OpenMP target teams directive. */
export const CXCursor_OMPTargetTeamsDirective: number = CXCursorKind.CXCursor_OMPTargetTeamsDirective;
/** OpenMP target teams distribute directive. */
export const CXCursor_OMPTargetTeamsDistributeDirective: number = CXCursorKind.CXCursor_OMPTargetTeamsDistributeDirective;
/** OpenMP target teams distribute parallel for directive. */
export const CXCursor_OMPTargetTeamsDistributeParallelForDirective: number = CXCursorKind.CXCursor_OMPTargetTeamsDistributeParallelForDirective;
/** OpenMP target teams distribute parallel for simd directive. */
export const CXCursor_OMPTargetTeamsDistributeParallelForSimdDirective: number = CXCursorKind.CXCursor_OMPTargetTeamsDistributeParallelForSimdDirective;
/** OpenMP target teams distribute simd directive. */
export const CXCursor_OMPTargetTeamsDistributeSimdDirective: number = CXCursorKind.CXCursor_OMPTargetTeamsDistributeSimdDirective;
/** C++2a std::bit_cast expression. */
export const CXCursor_BuiltinBitCastExpr: number = CXCursorKind.CXCursor_BuiltinBitCastExpr;
/** OpenMP master taskloop directive. */
export const CXCursor_OMPMasterTaskLoopDirective: number = CXCursorKind.CXCursor_OMPMasterTaskLoopDirective;
/** OpenMP parallel master taskloop directive. */
export const CXCursor_OMPParallelMasterTaskLoopDirective: number = CXCursorKind.CXCursor_OMPParallelMasterTaskLoopDirective;
/** OpenMP master taskloop simd directive. */
export const CXCursor_OMPMasterTaskLoopSimdDirective: number = CXCursorKind.CXCursor_OMPMasterTaskLoopSimdDirective;
/** OpenMP parallel master taskloop simd directive. */
export const CXCursor_OMPParallelMasterTaskLoopSimdDirective: number = CXCursorKind.CXCursor_OMPParallelMasterTaskLoopSimdDirective;
/** OpenMP parallel master directive. */
export const CXCursor_OMPParallelMasterDirective: number = CXCursorKind.CXCursor_OMPParallelMasterDirective;
/** OpenMP depobj directive. */
export const CXCursor_OMPDepobjDirective: number = CXCursorKind.CXCursor_OMPDepobjDirective;
/** OpenMP scan directive. */
export const CXCursor_OMPScanDirective: number = CXCursorKind.CXCursor_OMPScanDirective;
/** OpenMP tile directive. */
export const CXCursor_OMPTileDirective: number = CXCursorKind.CXCursor_OMPTileDirective;
/** OpenMP canonical loop. */
export const CXCursor_OMPCanonicalLoop: number = CXCursorKind.CXCursor_OMPCanonicalLoop;
/** OpenMP interop directive. */
export const CXCursor_OMPInteropDirective: number = CXCursorKind.CXCursor_OMPInteropDirective;
/** OpenMP dispatch directive. */
export const CXCursor_OMPDispatchDirective: number = CXCursorKind.CXCursor_OMPDispatchDirective;
/** OpenMP masked directive. */
export const CXCursor_OMPMaskedDirective: number = CXCursorKind.CXCursor_OMPMaskedDirective;
/** OpenMP unroll directive. */
export const CXCursor_OMPUnrollDirective: number = CXCursorKind.CXCursor_OMPUnrollDirective;
/** OpenMP metadirective directive. */
export const CXCursor_OMPMetaDirective: number = CXCursorKind.CXCursor_OMPMetaDirective;
/** OpenMP loop directive. */
export const CXCursor_OMPGenericLoopDirective: number = CXCursorKind.CXCursor_OMPGenericLoopDirective;
/** OpenMP teams loop directive. */
export const CXCursor_OMPTeamsGenericLoopDirective: number = CXCursorKind.CXCursor_OMPTeamsGenericLoopDirective;
/** OpenMP target teams loop directive. */
export const CXCursor_OMPTargetTeamsGenericLoopDirective: number = CXCursorKind.CXCursor_OMPTargetTeamsGenericLoopDirective;
/** OpenMP parallel loop directive. */
export const CXCursor_OMPParallelGenericLoopDirective: number = CXCursorKind.CXCursor_OMPParallelGenericLoopDirective;
/** OpenMP target parallel loop directive. */
export const CXCursor_OMPTargetParallelGenericLoopDirective: number = CXCursorKind.CXCursor_OMPTargetParallelGenericLoopDirective;
/** OpenMP parallel masked directive. */
export const CXCursor_OMPParallelMaskedDirective: number = CXCursorKind.CXCursor_OMPParallelMaskedDirective;
/** OpenMP masked taskloop directive. */
export const CXCursor_OMPMaskedTaskLoopDirective: number = CXCursorKind.CXCursor_OMPMaskedTaskLoopDirective;
/** OpenMP masked taskloop simd directive. */
export const CXCursor_OMPMaskedTaskLoopSimdDirective: number = CXCursorKind.CXCursor_OMPMaskedTaskLoopSimdDirective;
/** OpenMP parallel masked taskloop directive. */
export const CXCursor_OMPParallelMaskedTaskLoopDirective: number = CXCursorKind.CXCursor_OMPParallelMaskedTaskLoopDirective;
/** OpenMP parallel masked taskloop simd directive. */
export const CXCursor_OMPParallelMaskedTaskLoopSimdDirective: number = CXCursorKind.CXCursor_OMPParallelMaskedTaskLoopSimdDirective;
/** OpenMP error directive. */
export const CXCursor_OMPErrorDirective: number = CXCursorKind.CXCursor_OMPErrorDirective;
/** OpenMP scope directive. */
export const CXCursor_OMPScopeDirective: number = CXCursorKind.CXCursor_OMPScopeDirective;
/** OpenMP reverse directive. */
export const CXCursor_OMPReverseDirective: number = CXCursorKind.CXCursor_OMPReverseDirective;
/** OpenMP interchange directive. */
export const CXCursor_OMPInterchangeDirective: number = CXCursorKind.CXCursor_OMPInterchangeDirective;
/** OpenMP assume directive. */
export const CXCursor_OMPAssumeDirective: number = CXCursorKind.CXCursor_OMPAssumeDirective;
/** OpenACC Compute Construct. */
export const CXCursor_OpenACCComputeConstruct: number = CXCursorKind.CXCursor_OpenACCComputeConstruct;
/** OpenACC Loop Construct. */
export const CXCursor_OpenACCLoopConstruct: number = CXCursorKind.CXCursor_OpenACCLoopConstruct;
/** OpenACC Combined Constructs. */
export const CXCursor_OpenACCCombinedConstruct: number = CXCursorKind.CXCursor_OpenACCCombinedConstruct;
/** OpenACC data Construct. */
export const CXCursor_OpenACCDataConstruct: number = CXCursorKind.CXCursor_OpenACCDataConstruct;
/** OpenACC enter data Construct. */
export const CXCursor_OpenACCEnterDataConstruct: number = CXCursorKind.CXCursor_OpenACCEnterDataConstruct;
/** OpenACC exit data Construct. */
export const CXCursor_OpenACCExitDataConstruct: number = CXCursorKind.CXCursor_OpenACCExitDataConstruct;
/** OpenACC host_data Construct. */
export const CXCursor_OpenACCHostDataConstruct: number = CXCursorKind.CXCursor_OpenACCHostDataConstruct;
/** OpenACC wait Construct. */
export const CXCursor_OpenACCWaitConstruct: number = CXCursorKind.CXCursor_OpenACCWaitConstruct;
/** OpenACC init Construct. */
export const CXCursor_OpenACCInitConstruct: number = CXCursorKind.CXCursor_OpenACCInitConstruct;
/** OpenACC shutdown Construct. */
export const CXCursor_OpenACCShutdownConstruct: number = CXCursorKind.CXCursor_OpenACCShutdownConstruct;
/** OpenACC set Construct. */
export const CXCursor_OpenACCSetConstruct: number = CXCursorKind.CXCursor_OpenACCSetConstruct;
/** OpenACC update Construct. */
export const CXCursor_OpenACCUpdateConstruct: number = CXCursorKind.CXCursor_OpenACCUpdateConstruct;
/** OpenACC update Construct. */
export const CXCursor_LastStmt: number = CXCursorKind.CXCursor_LastStmt;
/** Cursor that represents the translation unit itself.

The translation unit cursor exists primarily to act as the root
cursor for traversing the contents of a translation unit. */
export const CXCursor_TranslationUnit: number = CXCursorKind.CXCursor_TranslationUnit;
/** Cursor that represents the translation unit itself.

The translation unit cursor exists primarily to act as the root
cursor for traversing the contents of a translation unit. */
export const CXCursor_FirstAttr: number = CXCursorKind.CXCursor_FirstAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_UnexposedAttr: number = CXCursorKind.CXCursor_UnexposedAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_IBActionAttr: number = CXCursorKind.CXCursor_IBActionAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_IBOutletAttr: number = CXCursorKind.CXCursor_IBOutletAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_IBOutletCollectionAttr: number = CXCursorKind.CXCursor_IBOutletCollectionAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_CXXFinalAttr: number = CXCursorKind.CXCursor_CXXFinalAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_CXXOverrideAttr: number = CXCursorKind.CXCursor_CXXOverrideAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_AnnotateAttr: number = CXCursorKind.CXCursor_AnnotateAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_AsmLabelAttr: number = CXCursorKind.CXCursor_AsmLabelAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_PackedAttr: number = CXCursorKind.CXCursor_PackedAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_PureAttr: number = CXCursorKind.CXCursor_PureAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_ConstAttr: number = CXCursorKind.CXCursor_ConstAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_NoDuplicateAttr: number = CXCursorKind.CXCursor_NoDuplicateAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_CUDAConstantAttr: number = CXCursorKind.CXCursor_CUDAConstantAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_CUDADeviceAttr: number = CXCursorKind.CXCursor_CUDADeviceAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_CUDAGlobalAttr: number = CXCursorKind.CXCursor_CUDAGlobalAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_CUDAHostAttr: number = CXCursorKind.CXCursor_CUDAHostAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_CUDASharedAttr: number = CXCursorKind.CXCursor_CUDASharedAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_VisibilityAttr: number = CXCursorKind.CXCursor_VisibilityAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_DLLExport: number = CXCursorKind.CXCursor_DLLExport;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_DLLImport: number = CXCursorKind.CXCursor_DLLImport;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_NSReturnsRetained: number = CXCursorKind.CXCursor_NSReturnsRetained;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_NSReturnsNotRetained: number = CXCursorKind.CXCursor_NSReturnsNotRetained;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_NSReturnsAutoreleased: number = CXCursorKind.CXCursor_NSReturnsAutoreleased;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_NSConsumesSelf: number = CXCursorKind.CXCursor_NSConsumesSelf;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_NSConsumed: number = CXCursorKind.CXCursor_NSConsumed;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_ObjCException: number = CXCursorKind.CXCursor_ObjCException;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_ObjCNSObject: number = CXCursorKind.CXCursor_ObjCNSObject;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_ObjCIndependentClass: number = CXCursorKind.CXCursor_ObjCIndependentClass;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_ObjCPreciseLifetime: number = CXCursorKind.CXCursor_ObjCPreciseLifetime;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_ObjCReturnsInnerPointer: number = CXCursorKind.CXCursor_ObjCReturnsInnerPointer;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_ObjCRequiresSuper: number = CXCursorKind.CXCursor_ObjCRequiresSuper;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_ObjCRootClass: number = CXCursorKind.CXCursor_ObjCRootClass;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_ObjCSubclassingRestricted: number = CXCursorKind.CXCursor_ObjCSubclassingRestricted;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_ObjCExplicitProtocolImpl: number = CXCursorKind.CXCursor_ObjCExplicitProtocolImpl;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_ObjCDesignatedInitializer: number = CXCursorKind.CXCursor_ObjCDesignatedInitializer;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_ObjCRuntimeVisible: number = CXCursorKind.CXCursor_ObjCRuntimeVisible;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_ObjCBoxable: number = CXCursorKind.CXCursor_ObjCBoxable;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_FlagEnum: number = CXCursorKind.CXCursor_FlagEnum;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_ConvergentAttr: number = CXCursorKind.CXCursor_ConvergentAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_WarnUnusedAttr: number = CXCursorKind.CXCursor_WarnUnusedAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_WarnUnusedResultAttr: number = CXCursorKind.CXCursor_WarnUnusedResultAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_AlignedAttr: number = CXCursorKind.CXCursor_AlignedAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_LastAttr: number = CXCursorKind.CXCursor_LastAttr;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_PreprocessingDirective: number = CXCursorKind.CXCursor_PreprocessingDirective;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_MacroDefinition: number = CXCursorKind.CXCursor_MacroDefinition;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_MacroExpansion: number = CXCursorKind.CXCursor_MacroExpansion;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_MacroInstantiation: number = CXCursorKind.CXCursor_MacroInstantiation;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_InclusionDirective: number = CXCursorKind.CXCursor_InclusionDirective;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_FirstPreprocessing: number = CXCursorKind.CXCursor_FirstPreprocessing;
/** An attribute whose specific kind is not exposed via this
interface. */
export const CXCursor_LastPreprocessing: number = CXCursorKind.CXCursor_LastPreprocessing;
/** A module import declaration. */
export const CXCursor_ModuleImportDecl: number = CXCursorKind.CXCursor_ModuleImportDecl;
/** A module import declaration. */
export const CXCursor_TypeAliasTemplateDecl: number = CXCursorKind.CXCursor_TypeAliasTemplateDecl;
/** A static_assert or _Static_assert node */
export const CXCursor_StaticAssert: number = CXCursorKind.CXCursor_StaticAssert;
/** a friend declaration. */
export const CXCursor_FriendDecl: number = CXCursorKind.CXCursor_FriendDecl;
/** a concept declaration. */
export const CXCursor_ConceptDecl: number = CXCursorKind.CXCursor_ConceptDecl;
/** a concept declaration. */
export const CXCursor_FirstExtraDecl: number = CXCursorKind.CXCursor_FirstExtraDecl;
/** a concept declaration. */
export const CXCursor_LastExtraDecl: number = CXCursorKind.CXCursor_LastExtraDecl;
/** A code completion overload candidate. */
export const CXCursor_OverloadCandidate: number = CXCursorKind.CXCursor_OverloadCandidate;

/**
 * Describe the linkage of the entity referred to by a cursor.
 */
export const CXLinkageKind = {
  /** This value indicates that no linkage information is available
for a provided CXCursor. */
  CXLinkage_Invalid: 0,
  /** This is the linkage for variables, parameters, and so on that
have automatic storage.  This covers normal (non-extern) local variables. */
  CXLinkage_NoLinkage: 1,
  /** This is the linkage for static variables and static functions. */
  CXLinkage_Internal: 2,
  /** This is the linkage for entities with external linkage that live
in C++ anonymous namespaces. */
  CXLinkage_UniqueExternal: 3,
  /** This is the linkage for entities with true, external linkage. */
  CXLinkage_External: 4,
} as const;

/** This value indicates that no linkage information is available
for a provided CXCursor. */
export const CXLinkage_Invalid: number = CXLinkageKind.CXLinkage_Invalid;
/** This is the linkage for variables, parameters, and so on that
have automatic storage.  This covers normal (non-extern) local variables. */
export const CXLinkage_NoLinkage: number = CXLinkageKind.CXLinkage_NoLinkage;
/** This is the linkage for static variables and static functions. */
export const CXLinkage_Internal: number = CXLinkageKind.CXLinkage_Internal;
/** This is the linkage for entities with external linkage that live
in C++ anonymous namespaces. */
export const CXLinkage_UniqueExternal: number = CXLinkageKind.CXLinkage_UniqueExternal;
/** This is the linkage for entities with true, external linkage. */
export const CXLinkage_External: number = CXLinkageKind.CXLinkage_External;

export const CXVisibilityKind = {
  /** This value indicates that no visibility information is available
for a provided CXCursor. */
  CXVisibility_Invalid: 0,
  /** Symbol not seen by the linker. */
  CXVisibility_Hidden: 1,
  /** Symbol seen by the linker but resolves to a symbol inside this object. */
  CXVisibility_Protected: 2,
  /** Symbol seen by the linker and acts like a normal symbol. */
  CXVisibility_Default: 3,
} as const;

/** This value indicates that no visibility information is available
for a provided CXCursor. */
export const CXVisibility_Invalid: number = CXVisibilityKind.CXVisibility_Invalid;
/** Symbol not seen by the linker. */
export const CXVisibility_Hidden: number = CXVisibilityKind.CXVisibility_Hidden;
/** Symbol seen by the linker but resolves to a symbol inside this object. */
export const CXVisibility_Protected: number = CXVisibilityKind.CXVisibility_Protected;
/** Symbol seen by the linker and acts like a normal symbol. */
export const CXVisibility_Default: number = CXVisibilityKind.CXVisibility_Default;

/**
 * Describe the "language" of the entity referred to by a cursor.
 */
export const CXLanguageKind = {
  CXLanguage_Invalid: 0,
  CXLanguage_C: 1,
  CXLanguage_ObjC: 2,
  CXLanguage_CPlusPlus: 3,
} as const;

export const CXLanguage_Invalid: number = CXLanguageKind.CXLanguage_Invalid;
export const CXLanguage_C: number = CXLanguageKind.CXLanguage_C;
export const CXLanguage_ObjC: number = CXLanguageKind.CXLanguage_ObjC;
export const CXLanguage_CPlusPlus: number = CXLanguageKind.CXLanguage_CPlusPlus;

/**
 * Describe the "thread-local storage (TLS) kind" of the declaration
referred to by a cursor.
 */
export const CXTLSKind = {
  CXTLS_None: 0,
  CXTLS_Dynamic: 1,
  CXTLS_Static: 2,
} as const;

export const CXTLS_None: number = CXTLSKind.CXTLS_None;
export const CXTLS_Dynamic: number = CXTLSKind.CXTLS_Dynamic;
export const CXTLS_Static: number = CXTLSKind.CXTLS_Static;

/**
 * Describes the kind of type
 */
export const CXTypeKind = {
  /** Represents an invalid type (e.g., where no type is available). */
  CXType_Invalid: 0,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Unexposed: 1,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Void: 2,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Bool: 3,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Char_U: 4,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_UChar: 5,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Char16: 6,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Char32: 7,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_UShort: 8,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_UInt: 9,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_ULong: 10,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_ULongLong: 11,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_UInt128: 12,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Char_S: 13,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_SChar: 14,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_WChar: 15,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Short: 16,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Int: 17,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Long: 18,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_LongLong: 19,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Int128: 20,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Float: 21,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Double: 22,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_LongDouble: 23,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_NullPtr: 24,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Overload: 25,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Dependent: 26,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_ObjCId: 27,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_ObjCClass: 28,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_ObjCSel: 29,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Float128: 30,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Half: 31,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Float16: 32,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_ShortAccum: 33,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Accum: 34,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_LongAccum: 35,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_UShortAccum: 36,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_UAccum: 37,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_ULongAccum: 38,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_BFloat16: 39,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Ibm128: 40,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_FirstBuiltin: 2,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_LastBuiltin: 40,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Complex: 100,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Pointer: 101,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_BlockPointer: 102,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_LValueReference: 103,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_RValueReference: 104,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Record: 105,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Enum: 106,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Typedef: 107,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_ObjCInterface: 108,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_ObjCObjectPointer: 109,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_FunctionNoProto: 110,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_FunctionProto: 111,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_ConstantArray: 112,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Vector: 113,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_IncompleteArray: 114,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_VariableArray: 115,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_DependentSizedArray: 116,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_MemberPointer: 117,
  /** A type whose specific kind is not exposed via this
interface. */
  CXType_Auto: 118,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_Elaborated: 119,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_Pipe: 120,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage1dRO: 121,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage1dArrayRO: 122,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage1dBufferRO: 123,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dRO: 124,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dArrayRO: 125,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dDepthRO: 126,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dArrayDepthRO: 127,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dMSAARO: 128,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dArrayMSAARO: 129,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dMSAADepthRO: 130,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dArrayMSAADepthRO: 131,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage3dRO: 132,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage1dWO: 133,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage1dArrayWO: 134,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage1dBufferWO: 135,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dWO: 136,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dArrayWO: 137,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dDepthWO: 138,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dArrayDepthWO: 139,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dMSAAWO: 140,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dArrayMSAAWO: 141,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dMSAADepthWO: 142,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dArrayMSAADepthWO: 143,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage3dWO: 144,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage1dRW: 145,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage1dArrayRW: 146,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage1dBufferRW: 147,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dRW: 148,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dArrayRW: 149,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dDepthRW: 150,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dArrayDepthRW: 151,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dMSAARW: 152,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dArrayMSAARW: 153,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dMSAADepthRW: 154,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage2dArrayMSAADepthRW: 155,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLImage3dRW: 156,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLSampler: 157,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLEvent: 158,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLQueue: 159,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLReserveID: 160,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_ObjCObject: 161,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_ObjCTypeParam: 162,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_Attributed: 163,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLIntelSubgroupAVCMcePayload: 164,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLIntelSubgroupAVCImePayload: 165,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLIntelSubgroupAVCRefPayload: 166,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLIntelSubgroupAVCSicPayload: 167,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLIntelSubgroupAVCMceResult: 168,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLIntelSubgroupAVCImeResult: 169,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLIntelSubgroupAVCRefResult: 170,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLIntelSubgroupAVCSicResult: 171,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLIntelSubgroupAVCImeResultSingleReferenceStreamout: 172,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLIntelSubgroupAVCImeResultDualReferenceStreamout: 173,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLIntelSubgroupAVCImeSingleReferenceStreamin: 174,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLIntelSubgroupAVCImeDualReferenceStreamin: 175,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLIntelSubgroupAVCImeResultSingleRefStreamout: 172,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLIntelSubgroupAVCImeResultDualRefStreamout: 173,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLIntelSubgroupAVCImeSingleRefStreamin: 174,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_OCLIntelSubgroupAVCImeDualRefStreamin: 175,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_ExtVector: 176,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_Atomic: 177,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_BTFTagAttributed: 178,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_HLSLResource: 179,
  /** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
  CXType_HLSLAttributedResource: 180,
} as const;

/** Represents an invalid type (e.g., where no type is available). */
export const CXType_Invalid: number = CXTypeKind.CXType_Invalid;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Unexposed: number = CXTypeKind.CXType_Unexposed;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Void: number = CXTypeKind.CXType_Void;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Bool: number = CXTypeKind.CXType_Bool;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Char_U: number = CXTypeKind.CXType_Char_U;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_UChar: number = CXTypeKind.CXType_UChar;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Char16: number = CXTypeKind.CXType_Char16;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Char32: number = CXTypeKind.CXType_Char32;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_UShort: number = CXTypeKind.CXType_UShort;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_UInt: number = CXTypeKind.CXType_UInt;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_ULong: number = CXTypeKind.CXType_ULong;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_ULongLong: number = CXTypeKind.CXType_ULongLong;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_UInt128: number = CXTypeKind.CXType_UInt128;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Char_S: number = CXTypeKind.CXType_Char_S;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_SChar: number = CXTypeKind.CXType_SChar;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_WChar: number = CXTypeKind.CXType_WChar;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Short: number = CXTypeKind.CXType_Short;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Int: number = CXTypeKind.CXType_Int;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Long: number = CXTypeKind.CXType_Long;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_LongLong: number = CXTypeKind.CXType_LongLong;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Int128: number = CXTypeKind.CXType_Int128;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Float: number = CXTypeKind.CXType_Float;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Double: number = CXTypeKind.CXType_Double;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_LongDouble: number = CXTypeKind.CXType_LongDouble;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_NullPtr: number = CXTypeKind.CXType_NullPtr;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Overload: number = CXTypeKind.CXType_Overload;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Dependent: number = CXTypeKind.CXType_Dependent;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_ObjCId: number = CXTypeKind.CXType_ObjCId;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_ObjCClass: number = CXTypeKind.CXType_ObjCClass;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_ObjCSel: number = CXTypeKind.CXType_ObjCSel;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Float128: number = CXTypeKind.CXType_Float128;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Half: number = CXTypeKind.CXType_Half;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Float16: number = CXTypeKind.CXType_Float16;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_ShortAccum: number = CXTypeKind.CXType_ShortAccum;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Accum: number = CXTypeKind.CXType_Accum;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_LongAccum: number = CXTypeKind.CXType_LongAccum;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_UShortAccum: number = CXTypeKind.CXType_UShortAccum;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_UAccum: number = CXTypeKind.CXType_UAccum;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_ULongAccum: number = CXTypeKind.CXType_ULongAccum;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_BFloat16: number = CXTypeKind.CXType_BFloat16;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Ibm128: number = CXTypeKind.CXType_Ibm128;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_FirstBuiltin: number = CXTypeKind.CXType_FirstBuiltin;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_LastBuiltin: number = CXTypeKind.CXType_LastBuiltin;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Complex: number = CXTypeKind.CXType_Complex;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Pointer: number = CXTypeKind.CXType_Pointer;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_BlockPointer: number = CXTypeKind.CXType_BlockPointer;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_LValueReference: number = CXTypeKind.CXType_LValueReference;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_RValueReference: number = CXTypeKind.CXType_RValueReference;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Record: number = CXTypeKind.CXType_Record;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Enum: number = CXTypeKind.CXType_Enum;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Typedef: number = CXTypeKind.CXType_Typedef;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_ObjCInterface: number = CXTypeKind.CXType_ObjCInterface;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_ObjCObjectPointer: number = CXTypeKind.CXType_ObjCObjectPointer;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_FunctionNoProto: number = CXTypeKind.CXType_FunctionNoProto;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_FunctionProto: number = CXTypeKind.CXType_FunctionProto;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_ConstantArray: number = CXTypeKind.CXType_ConstantArray;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Vector: number = CXTypeKind.CXType_Vector;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_IncompleteArray: number = CXTypeKind.CXType_IncompleteArray;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_VariableArray: number = CXTypeKind.CXType_VariableArray;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_DependentSizedArray: number = CXTypeKind.CXType_DependentSizedArray;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_MemberPointer: number = CXTypeKind.CXType_MemberPointer;
/** A type whose specific kind is not exposed via this
interface. */
export const CXType_Auto: number = CXTypeKind.CXType_Auto;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_Elaborated: number = CXTypeKind.CXType_Elaborated;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_Pipe: number = CXTypeKind.CXType_Pipe;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage1dRO: number = CXTypeKind.CXType_OCLImage1dRO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage1dArrayRO: number = CXTypeKind.CXType_OCLImage1dArrayRO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage1dBufferRO: number = CXTypeKind.CXType_OCLImage1dBufferRO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dRO: number = CXTypeKind.CXType_OCLImage2dRO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dArrayRO: number = CXTypeKind.CXType_OCLImage2dArrayRO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dDepthRO: number = CXTypeKind.CXType_OCLImage2dDepthRO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dArrayDepthRO: number = CXTypeKind.CXType_OCLImage2dArrayDepthRO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dMSAARO: number = CXTypeKind.CXType_OCLImage2dMSAARO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dArrayMSAARO: number = CXTypeKind.CXType_OCLImage2dArrayMSAARO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dMSAADepthRO: number = CXTypeKind.CXType_OCLImage2dMSAADepthRO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dArrayMSAADepthRO: number = CXTypeKind.CXType_OCLImage2dArrayMSAADepthRO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage3dRO: number = CXTypeKind.CXType_OCLImage3dRO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage1dWO: number = CXTypeKind.CXType_OCLImage1dWO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage1dArrayWO: number = CXTypeKind.CXType_OCLImage1dArrayWO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage1dBufferWO: number = CXTypeKind.CXType_OCLImage1dBufferWO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dWO: number = CXTypeKind.CXType_OCLImage2dWO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dArrayWO: number = CXTypeKind.CXType_OCLImage2dArrayWO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dDepthWO: number = CXTypeKind.CXType_OCLImage2dDepthWO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dArrayDepthWO: number = CXTypeKind.CXType_OCLImage2dArrayDepthWO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dMSAAWO: number = CXTypeKind.CXType_OCLImage2dMSAAWO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dArrayMSAAWO: number = CXTypeKind.CXType_OCLImage2dArrayMSAAWO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dMSAADepthWO: number = CXTypeKind.CXType_OCLImage2dMSAADepthWO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dArrayMSAADepthWO: number = CXTypeKind.CXType_OCLImage2dArrayMSAADepthWO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage3dWO: number = CXTypeKind.CXType_OCLImage3dWO;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage1dRW: number = CXTypeKind.CXType_OCLImage1dRW;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage1dArrayRW: number = CXTypeKind.CXType_OCLImage1dArrayRW;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage1dBufferRW: number = CXTypeKind.CXType_OCLImage1dBufferRW;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dRW: number = CXTypeKind.CXType_OCLImage2dRW;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dArrayRW: number = CXTypeKind.CXType_OCLImage2dArrayRW;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dDepthRW: number = CXTypeKind.CXType_OCLImage2dDepthRW;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dArrayDepthRW: number = CXTypeKind.CXType_OCLImage2dArrayDepthRW;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dMSAARW: number = CXTypeKind.CXType_OCLImage2dMSAARW;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dArrayMSAARW: number = CXTypeKind.CXType_OCLImage2dArrayMSAARW;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dMSAADepthRW: number = CXTypeKind.CXType_OCLImage2dMSAADepthRW;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage2dArrayMSAADepthRW: number = CXTypeKind.CXType_OCLImage2dArrayMSAADepthRW;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLImage3dRW: number = CXTypeKind.CXType_OCLImage3dRW;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLSampler: number = CXTypeKind.CXType_OCLSampler;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLEvent: number = CXTypeKind.CXType_OCLEvent;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLQueue: number = CXTypeKind.CXType_OCLQueue;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLReserveID: number = CXTypeKind.CXType_OCLReserveID;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_ObjCObject: number = CXTypeKind.CXType_ObjCObject;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_ObjCTypeParam: number = CXTypeKind.CXType_ObjCTypeParam;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_Attributed: number = CXTypeKind.CXType_Attributed;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLIntelSubgroupAVCMcePayload: number = CXTypeKind.CXType_OCLIntelSubgroupAVCMcePayload;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLIntelSubgroupAVCImePayload: number = CXTypeKind.CXType_OCLIntelSubgroupAVCImePayload;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLIntelSubgroupAVCRefPayload: number = CXTypeKind.CXType_OCLIntelSubgroupAVCRefPayload;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLIntelSubgroupAVCSicPayload: number = CXTypeKind.CXType_OCLIntelSubgroupAVCSicPayload;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLIntelSubgroupAVCMceResult: number = CXTypeKind.CXType_OCLIntelSubgroupAVCMceResult;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLIntelSubgroupAVCImeResult: number = CXTypeKind.CXType_OCLIntelSubgroupAVCImeResult;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLIntelSubgroupAVCRefResult: number = CXTypeKind.CXType_OCLIntelSubgroupAVCRefResult;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLIntelSubgroupAVCSicResult: number = CXTypeKind.CXType_OCLIntelSubgroupAVCSicResult;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLIntelSubgroupAVCImeResultSingleReferenceStreamout: number = CXTypeKind.CXType_OCLIntelSubgroupAVCImeResultSingleReferenceStreamout;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLIntelSubgroupAVCImeResultDualReferenceStreamout: number = CXTypeKind.CXType_OCLIntelSubgroupAVCImeResultDualReferenceStreamout;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLIntelSubgroupAVCImeSingleReferenceStreamin: number = CXTypeKind.CXType_OCLIntelSubgroupAVCImeSingleReferenceStreamin;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLIntelSubgroupAVCImeDualReferenceStreamin: number = CXTypeKind.CXType_OCLIntelSubgroupAVCImeDualReferenceStreamin;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLIntelSubgroupAVCImeResultSingleRefStreamout: number = CXTypeKind.CXType_OCLIntelSubgroupAVCImeResultSingleRefStreamout;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLIntelSubgroupAVCImeResultDualRefStreamout: number = CXTypeKind.CXType_OCLIntelSubgroupAVCImeResultDualRefStreamout;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLIntelSubgroupAVCImeSingleRefStreamin: number = CXTypeKind.CXType_OCLIntelSubgroupAVCImeSingleRefStreamin;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_OCLIntelSubgroupAVCImeDualRefStreamin: number = CXTypeKind.CXType_OCLIntelSubgroupAVCImeDualRefStreamin;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_ExtVector: number = CXTypeKind.CXType_ExtVector;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_Atomic: number = CXTypeKind.CXType_Atomic;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_BTFTagAttributed: number = CXTypeKind.CXType_BTFTagAttributed;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_HLSLResource: number = CXTypeKind.CXType_HLSLResource;
/** Represents a type that was referred to using an elaborated type keyword.

E.g., struct S, or via a qualified name, e.g., N::M::type, or both. */
export const CXType_HLSLAttributedResource: number = CXTypeKind.CXType_HLSLAttributedResource;

/**
 * Describes the calling convention of a function type
 */
export const CXCallingConv = {
  CXCallingConv_Default: 0,
  CXCallingConv_C: 1,
  CXCallingConv_X86StdCall: 2,
  CXCallingConv_X86FastCall: 3,
  CXCallingConv_X86ThisCall: 4,
  CXCallingConv_X86Pascal: 5,
  CXCallingConv_AAPCS: 6,
  CXCallingConv_AAPCS_VFP: 7,
  CXCallingConv_X86RegCall: 8,
  CXCallingConv_IntelOclBicc: 9,
  CXCallingConv_Win64: 10,
  CXCallingConv_X86_64Win64: 10,
  CXCallingConv_X86_64SysV: 11,
  CXCallingConv_X86VectorCall: 12,
  CXCallingConv_Swift: 13,
  CXCallingConv_PreserveMost: 14,
  CXCallingConv_PreserveAll: 15,
  CXCallingConv_AArch64VectorCall: 16,
  CXCallingConv_SwiftAsync: 17,
  CXCallingConv_AArch64SVEPCS: 18,
  CXCallingConv_M68kRTD: 19,
  CXCallingConv_PreserveNone: 20,
  CXCallingConv_RISCVVectorCall: 21,
  CXCallingConv_Invalid: 100,
  CXCallingConv_Unexposed: 200,
} as const;

export const CXCallingConv_Default: number = CXCallingConv.CXCallingConv_Default;
export const CXCallingConv_C: number = CXCallingConv.CXCallingConv_C;
export const CXCallingConv_X86StdCall: number = CXCallingConv.CXCallingConv_X86StdCall;
export const CXCallingConv_X86FastCall: number = CXCallingConv.CXCallingConv_X86FastCall;
export const CXCallingConv_X86ThisCall: number = CXCallingConv.CXCallingConv_X86ThisCall;
export const CXCallingConv_X86Pascal: number = CXCallingConv.CXCallingConv_X86Pascal;
export const CXCallingConv_AAPCS: number = CXCallingConv.CXCallingConv_AAPCS;
export const CXCallingConv_AAPCS_VFP: number = CXCallingConv.CXCallingConv_AAPCS_VFP;
export const CXCallingConv_X86RegCall: number = CXCallingConv.CXCallingConv_X86RegCall;
export const CXCallingConv_IntelOclBicc: number = CXCallingConv.CXCallingConv_IntelOclBicc;
export const CXCallingConv_Win64: number = CXCallingConv.CXCallingConv_Win64;
export const CXCallingConv_X86_64Win64: number = CXCallingConv.CXCallingConv_X86_64Win64;
export const CXCallingConv_X86_64SysV: number = CXCallingConv.CXCallingConv_X86_64SysV;
export const CXCallingConv_X86VectorCall: number = CXCallingConv.CXCallingConv_X86VectorCall;
export const CXCallingConv_Swift: number = CXCallingConv.CXCallingConv_Swift;
export const CXCallingConv_PreserveMost: number = CXCallingConv.CXCallingConv_PreserveMost;
export const CXCallingConv_PreserveAll: number = CXCallingConv.CXCallingConv_PreserveAll;
export const CXCallingConv_AArch64VectorCall: number = CXCallingConv.CXCallingConv_AArch64VectorCall;
export const CXCallingConv_SwiftAsync: number = CXCallingConv.CXCallingConv_SwiftAsync;
export const CXCallingConv_AArch64SVEPCS: number = CXCallingConv.CXCallingConv_AArch64SVEPCS;
export const CXCallingConv_M68kRTD: number = CXCallingConv.CXCallingConv_M68kRTD;
export const CXCallingConv_PreserveNone: number = CXCallingConv.CXCallingConv_PreserveNone;
export const CXCallingConv_RISCVVectorCall: number = CXCallingConv.CXCallingConv_RISCVVectorCall;
export const CXCallingConv_Invalid: number = CXCallingConv.CXCallingConv_Invalid;
export const CXCallingConv_Unexposed: number = CXCallingConv.CXCallingConv_Unexposed;

/**
 * Describes the kind of a template argument.

See the definition of llvm::clang::TemplateArgument::ArgKind for full
element descriptions.
 */
export const CXTemplateArgumentKind = {
  CXTemplateArgumentKind_Null: 0,
  CXTemplateArgumentKind_Type: 1,
  CXTemplateArgumentKind_Declaration: 2,
  CXTemplateArgumentKind_NullPtr: 3,
  CXTemplateArgumentKind_Integral: 4,
  CXTemplateArgumentKind_Template: 5,
  CXTemplateArgumentKind_TemplateExpansion: 6,
  CXTemplateArgumentKind_Expression: 7,
  CXTemplateArgumentKind_Pack: 8,
  CXTemplateArgumentKind_Invalid: 9,
} as const;

export const CXTemplateArgumentKind_Null: number = CXTemplateArgumentKind.CXTemplateArgumentKind_Null;
export const CXTemplateArgumentKind_Type: number = CXTemplateArgumentKind.CXTemplateArgumentKind_Type;
export const CXTemplateArgumentKind_Declaration: number = CXTemplateArgumentKind.CXTemplateArgumentKind_Declaration;
export const CXTemplateArgumentKind_NullPtr: number = CXTemplateArgumentKind.CXTemplateArgumentKind_NullPtr;
export const CXTemplateArgumentKind_Integral: number = CXTemplateArgumentKind.CXTemplateArgumentKind_Integral;
export const CXTemplateArgumentKind_Template: number = CXTemplateArgumentKind.CXTemplateArgumentKind_Template;
export const CXTemplateArgumentKind_TemplateExpansion: number = CXTemplateArgumentKind.CXTemplateArgumentKind_TemplateExpansion;
export const CXTemplateArgumentKind_Expression: number = CXTemplateArgumentKind.CXTemplateArgumentKind_Expression;
export const CXTemplateArgumentKind_Pack: number = CXTemplateArgumentKind.CXTemplateArgumentKind_Pack;
export const CXTemplateArgumentKind_Invalid: number = CXTemplateArgumentKind.CXTemplateArgumentKind_Invalid;

export const CXTypeNullabilityKind = {
  /** Values of this type can never be null. */
  CXTypeNullability_NonNull: 0,
  /** Values of this type can be null. */
  CXTypeNullability_Nullable: 1,
  /** Whether values of this type can be null is (explicitly)
unspecified. This captures a (fairly rare) case where we
can't conclude anything about the nullability of the type even
though it has been considered. */
  CXTypeNullability_Unspecified: 2,
  /** Nullability is not applicable to this type. */
  CXTypeNullability_Invalid: 3,
  /** Generally behaves like Nullable, except when used in a block parameter that
was imported into a swift async method. There, swift will assume that the
parameter can get null even if no error occurred. _Nullable parameters are
assumed to only get null on error. */
  CXTypeNullability_NullableResult: 4,
} as const;

/** Values of this type can never be null. */
export const CXTypeNullability_NonNull: number = CXTypeNullabilityKind.CXTypeNullability_NonNull;
/** Values of this type can be null. */
export const CXTypeNullability_Nullable: number = CXTypeNullabilityKind.CXTypeNullability_Nullable;
/** Whether values of this type can be null is (explicitly)
unspecified. This captures a (fairly rare) case where we
can't conclude anything about the nullability of the type even
though it has been considered. */
export const CXTypeNullability_Unspecified: number = CXTypeNullabilityKind.CXTypeNullability_Unspecified;
/** Nullability is not applicable to this type. */
export const CXTypeNullability_Invalid: number = CXTypeNullabilityKind.CXTypeNullability_Invalid;
/** Generally behaves like Nullable, except when used in a block parameter that
was imported into a swift async method. There, swift will assume that the
parameter can get null even if no error occurred. _Nullable parameters are
assumed to only get null on error. */
export const CXTypeNullability_NullableResult: number = CXTypeNullabilityKind.CXTypeNullability_NullableResult;

/**
 * List the possible error codes for \c clang_Type_getSizeOf,
\c clang_Type_getAlignOf, \c clang_Type_getOffsetOf,
\c clang_Cursor_getOffsetOf, and \c clang_getOffsetOfBase.

A value of this enumeration type can be returned if the target type is not
a valid argument to sizeof, alignof or offsetof.
 */
export const CXTypeLayoutError = {
  /** Type is of kind CXType_Invalid. */
  CXTypeLayoutError_Invalid: -1,
  /** The type is an incomplete Type. */
  CXTypeLayoutError_Incomplete: -2,
  /** The type is a dependent Type. */
  CXTypeLayoutError_Dependent: -3,
  /** The type is not a constant size type. */
  CXTypeLayoutError_NotConstantSize: -4,
  /** The Field name is not valid for this record. */
  CXTypeLayoutError_InvalidFieldName: -5,
  /** The type is undeduced. */
  CXTypeLayoutError_Undeduced: -6,
} as const;

/** Type is of kind CXType_Invalid. */
export const CXTypeLayoutError_Invalid: number = CXTypeLayoutError.CXTypeLayoutError_Invalid;
/** The type is an incomplete Type. */
export const CXTypeLayoutError_Incomplete: number = CXTypeLayoutError.CXTypeLayoutError_Incomplete;
/** The type is a dependent Type. */
export const CXTypeLayoutError_Dependent: number = CXTypeLayoutError.CXTypeLayoutError_Dependent;
/** The type is not a constant size type. */
export const CXTypeLayoutError_NotConstantSize: number = CXTypeLayoutError.CXTypeLayoutError_NotConstantSize;
/** The Field name is not valid for this record. */
export const CXTypeLayoutError_InvalidFieldName: number = CXTypeLayoutError.CXTypeLayoutError_InvalidFieldName;
/** The type is undeduced. */
export const CXTypeLayoutError_Undeduced: number = CXTypeLayoutError.CXTypeLayoutError_Undeduced;

export const CXRefQualifierKind = {
  /** No ref-qualifier was provided. */
  CXRefQualifier_None: 0,
  /** An lvalue ref-qualifier was provided (\c &). */
  CXRefQualifier_LValue: 1,
  /** An rvalue ref-qualifier was provided (\c &&). */
  CXRefQualifier_RValue: 2,
} as const;

/** No ref-qualifier was provided. */
export const CXRefQualifier_None: number = CXRefQualifierKind.CXRefQualifier_None;
/** An lvalue ref-qualifier was provided (\c &). */
export const CXRefQualifier_LValue: number = CXRefQualifierKind.CXRefQualifier_LValue;
/** An rvalue ref-qualifier was provided (\c &&). */
export const CXRefQualifier_RValue: number = CXRefQualifierKind.CXRefQualifier_RValue;

/**
 * Represents the C++ access control level to a base class for a
cursor with kind CX_CXXBaseSpecifier.
 */
export const CX_CXXAccessSpecifier = {
  CX_CXXInvalidAccessSpecifier: 0,
  CX_CXXPublic: 1,
  CX_CXXProtected: 2,
  CX_CXXPrivate: 3,
} as const;

export const CX_CXXInvalidAccessSpecifier: number = CX_CXXAccessSpecifier.CX_CXXInvalidAccessSpecifier;
export const CX_CXXPublic: number = CX_CXXAccessSpecifier.CX_CXXPublic;
export const CX_CXXProtected: number = CX_CXXAccessSpecifier.CX_CXXProtected;
export const CX_CXXPrivate: number = CX_CXXAccessSpecifier.CX_CXXPrivate;

/**
 * Represents the storage classes as declared in the source. CX_SC_Invalid
was added for the case that the passed cursor in not a declaration.
 */
export const CX_StorageClass = {
  CX_SC_Invalid: 0,
  CX_SC_None: 1,
  CX_SC_Extern: 2,
  CX_SC_Static: 3,
  CX_SC_PrivateExtern: 4,
  CX_SC_OpenCLWorkGroupLocal: 5,
  CX_SC_Auto: 6,
  CX_SC_Register: 7,
} as const;

export const CX_SC_Invalid: number = CX_StorageClass.CX_SC_Invalid;
export const CX_SC_None: number = CX_StorageClass.CX_SC_None;
export const CX_SC_Extern: number = CX_StorageClass.CX_SC_Extern;
export const CX_SC_Static: number = CX_StorageClass.CX_SC_Static;
export const CX_SC_PrivateExtern: number = CX_StorageClass.CX_SC_PrivateExtern;
export const CX_SC_OpenCLWorkGroupLocal: number = CX_StorageClass.CX_SC_OpenCLWorkGroupLocal;
export const CX_SC_Auto: number = CX_StorageClass.CX_SC_Auto;
export const CX_SC_Register: number = CX_StorageClass.CX_SC_Register;

/**
 * Represents a specific kind of binary operator which can appear at a cursor.
 */
export const CX_BinaryOperatorKind = {
  CX_BO_Invalid: 0,
  CX_BO_PtrMemD: 1,
  CX_BO_PtrMemI: 2,
  CX_BO_Mul: 3,
  CX_BO_Div: 4,
  CX_BO_Rem: 5,
  CX_BO_Add: 6,
  CX_BO_Sub: 7,
  CX_BO_Shl: 8,
  CX_BO_Shr: 9,
  CX_BO_Cmp: 10,
  CX_BO_LT: 11,
  CX_BO_GT: 12,
  CX_BO_LE: 13,
  CX_BO_GE: 14,
  CX_BO_EQ: 15,
  CX_BO_NE: 16,
  CX_BO_And: 17,
  CX_BO_Xor: 18,
  CX_BO_Or: 19,
  CX_BO_LAnd: 20,
  CX_BO_LOr: 21,
  CX_BO_Assign: 22,
  CX_BO_MulAssign: 23,
  CX_BO_DivAssign: 24,
  CX_BO_RemAssign: 25,
  CX_BO_AddAssign: 26,
  CX_BO_SubAssign: 27,
  CX_BO_ShlAssign: 28,
  CX_BO_ShrAssign: 29,
  CX_BO_AndAssign: 30,
  CX_BO_XorAssign: 31,
  CX_BO_OrAssign: 32,
  CX_BO_Comma: 33,
  CX_BO_LAST: 33,
} as const;

export const CX_BO_Invalid: number = CX_BinaryOperatorKind.CX_BO_Invalid;
export const CX_BO_PtrMemD: number = CX_BinaryOperatorKind.CX_BO_PtrMemD;
export const CX_BO_PtrMemI: number = CX_BinaryOperatorKind.CX_BO_PtrMemI;
export const CX_BO_Mul: number = CX_BinaryOperatorKind.CX_BO_Mul;
export const CX_BO_Div: number = CX_BinaryOperatorKind.CX_BO_Div;
export const CX_BO_Rem: number = CX_BinaryOperatorKind.CX_BO_Rem;
export const CX_BO_Add: number = CX_BinaryOperatorKind.CX_BO_Add;
export const CX_BO_Sub: number = CX_BinaryOperatorKind.CX_BO_Sub;
export const CX_BO_Shl: number = CX_BinaryOperatorKind.CX_BO_Shl;
export const CX_BO_Shr: number = CX_BinaryOperatorKind.CX_BO_Shr;
export const CX_BO_Cmp: number = CX_BinaryOperatorKind.CX_BO_Cmp;
export const CX_BO_LT: number = CX_BinaryOperatorKind.CX_BO_LT;
export const CX_BO_GT: number = CX_BinaryOperatorKind.CX_BO_GT;
export const CX_BO_LE: number = CX_BinaryOperatorKind.CX_BO_LE;
export const CX_BO_GE: number = CX_BinaryOperatorKind.CX_BO_GE;
export const CX_BO_EQ: number = CX_BinaryOperatorKind.CX_BO_EQ;
export const CX_BO_NE: number = CX_BinaryOperatorKind.CX_BO_NE;
export const CX_BO_And: number = CX_BinaryOperatorKind.CX_BO_And;
export const CX_BO_Xor: number = CX_BinaryOperatorKind.CX_BO_Xor;
export const CX_BO_Or: number = CX_BinaryOperatorKind.CX_BO_Or;
export const CX_BO_LAnd: number = CX_BinaryOperatorKind.CX_BO_LAnd;
export const CX_BO_LOr: number = CX_BinaryOperatorKind.CX_BO_LOr;
export const CX_BO_Assign: number = CX_BinaryOperatorKind.CX_BO_Assign;
export const CX_BO_MulAssign: number = CX_BinaryOperatorKind.CX_BO_MulAssign;
export const CX_BO_DivAssign: number = CX_BinaryOperatorKind.CX_BO_DivAssign;
export const CX_BO_RemAssign: number = CX_BinaryOperatorKind.CX_BO_RemAssign;
export const CX_BO_AddAssign: number = CX_BinaryOperatorKind.CX_BO_AddAssign;
export const CX_BO_SubAssign: number = CX_BinaryOperatorKind.CX_BO_SubAssign;
export const CX_BO_ShlAssign: number = CX_BinaryOperatorKind.CX_BO_ShlAssign;
export const CX_BO_ShrAssign: number = CX_BinaryOperatorKind.CX_BO_ShrAssign;
export const CX_BO_AndAssign: number = CX_BinaryOperatorKind.CX_BO_AndAssign;
export const CX_BO_XorAssign: number = CX_BinaryOperatorKind.CX_BO_XorAssign;
export const CX_BO_OrAssign: number = CX_BinaryOperatorKind.CX_BO_OrAssign;
export const CX_BO_Comma: number = CX_BinaryOperatorKind.CX_BO_Comma;
export const CX_BO_LAST: number = CX_BinaryOperatorKind.CX_BO_LAST;

/**
 * Describes how the traversal of the children of a particular
cursor should proceed after visiting a particular child cursor.

A value of this enumeration type should be returned by each
\c CXCursorVisitor to indicate how clang_visitChildren() proceed.
 */
export const CXChildVisitResult = {
  /** Terminates the cursor traversal. */
  CXChildVisit_Break: 0,
  /** Continues the cursor traversal with the next sibling of
the cursor just visited, without visiting its children. */
  CXChildVisit_Continue: 1,
  /** Recursively traverse the children of this cursor, using
the same visitor and client data. */
  CXChildVisit_Recurse: 2,
} as const;

/** Terminates the cursor traversal. */
export const CXChildVisit_Break: number = CXChildVisitResult.CXChildVisit_Break;
/** Continues the cursor traversal with the next sibling of
the cursor just visited, without visiting its children. */
export const CXChildVisit_Continue: number = CXChildVisitResult.CXChildVisit_Continue;
/** Recursively traverse the children of this cursor, using
the same visitor and client data. */
export const CXChildVisit_Recurse: number = CXChildVisitResult.CXChildVisit_Recurse;

/**
 * Properties for the printing policy.

See \c clang::PrintingPolicy for more information.
 */
export const CXPrintingPolicyProperty = {
  CXPrintingPolicy_Indentation: 0,
  CXPrintingPolicy_SuppressSpecifiers: 1,
  CXPrintingPolicy_SuppressTagKeyword: 2,
  CXPrintingPolicy_IncludeTagDefinition: 3,
  CXPrintingPolicy_SuppressScope: 4,
  CXPrintingPolicy_SuppressUnwrittenScope: 5,
  CXPrintingPolicy_SuppressInitializers: 6,
  CXPrintingPolicy_ConstantArraySizeAsWritten: 7,
  CXPrintingPolicy_AnonymousTagLocations: 8,
  CXPrintingPolicy_SuppressStrongLifetime: 9,
  CXPrintingPolicy_SuppressLifetimeQualifiers: 10,
  CXPrintingPolicy_SuppressTemplateArgsInCXXConstructors: 11,
  CXPrintingPolicy_Bool: 12,
  CXPrintingPolicy_Restrict: 13,
  CXPrintingPolicy_Alignof: 14,
  CXPrintingPolicy_UnderscoreAlignof: 15,
  CXPrintingPolicy_UseVoidForZeroParams: 16,
  CXPrintingPolicy_TerseOutput: 17,
  CXPrintingPolicy_PolishForDeclaration: 18,
  CXPrintingPolicy_Half: 19,
  CXPrintingPolicy_MSWChar: 20,
  CXPrintingPolicy_IncludeNewlines: 21,
  CXPrintingPolicy_MSVCFormatting: 22,
  CXPrintingPolicy_ConstantsAsWritten: 23,
  CXPrintingPolicy_SuppressImplicitBase: 24,
  CXPrintingPolicy_FullyQualifiedName: 25,
  CXPrintingPolicy_LastProperty: 25,
} as const;

export const CXPrintingPolicy_Indentation: number = CXPrintingPolicyProperty.CXPrintingPolicy_Indentation;
export const CXPrintingPolicy_SuppressSpecifiers: number = CXPrintingPolicyProperty.CXPrintingPolicy_SuppressSpecifiers;
export const CXPrintingPolicy_SuppressTagKeyword: number = CXPrintingPolicyProperty.CXPrintingPolicy_SuppressTagKeyword;
export const CXPrintingPolicy_IncludeTagDefinition: number = CXPrintingPolicyProperty.CXPrintingPolicy_IncludeTagDefinition;
export const CXPrintingPolicy_SuppressScope: number = CXPrintingPolicyProperty.CXPrintingPolicy_SuppressScope;
export const CXPrintingPolicy_SuppressUnwrittenScope: number = CXPrintingPolicyProperty.CXPrintingPolicy_SuppressUnwrittenScope;
export const CXPrintingPolicy_SuppressInitializers: number = CXPrintingPolicyProperty.CXPrintingPolicy_SuppressInitializers;
export const CXPrintingPolicy_ConstantArraySizeAsWritten: number = CXPrintingPolicyProperty.CXPrintingPolicy_ConstantArraySizeAsWritten;
export const CXPrintingPolicy_AnonymousTagLocations: number = CXPrintingPolicyProperty.CXPrintingPolicy_AnonymousTagLocations;
export const CXPrintingPolicy_SuppressStrongLifetime: number = CXPrintingPolicyProperty.CXPrintingPolicy_SuppressStrongLifetime;
export const CXPrintingPolicy_SuppressLifetimeQualifiers: number = CXPrintingPolicyProperty.CXPrintingPolicy_SuppressLifetimeQualifiers;
export const CXPrintingPolicy_SuppressTemplateArgsInCXXConstructors: number = CXPrintingPolicyProperty.CXPrintingPolicy_SuppressTemplateArgsInCXXConstructors;
export const CXPrintingPolicy_Bool: number = CXPrintingPolicyProperty.CXPrintingPolicy_Bool;
export const CXPrintingPolicy_Restrict: number = CXPrintingPolicyProperty.CXPrintingPolicy_Restrict;
export const CXPrintingPolicy_Alignof: number = CXPrintingPolicyProperty.CXPrintingPolicy_Alignof;
export const CXPrintingPolicy_UnderscoreAlignof: number = CXPrintingPolicyProperty.CXPrintingPolicy_UnderscoreAlignof;
export const CXPrintingPolicy_UseVoidForZeroParams: number = CXPrintingPolicyProperty.CXPrintingPolicy_UseVoidForZeroParams;
export const CXPrintingPolicy_TerseOutput: number = CXPrintingPolicyProperty.CXPrintingPolicy_TerseOutput;
export const CXPrintingPolicy_PolishForDeclaration: number = CXPrintingPolicyProperty.CXPrintingPolicy_PolishForDeclaration;
export const CXPrintingPolicy_Half: number = CXPrintingPolicyProperty.CXPrintingPolicy_Half;
export const CXPrintingPolicy_MSWChar: number = CXPrintingPolicyProperty.CXPrintingPolicy_MSWChar;
export const CXPrintingPolicy_IncludeNewlines: number = CXPrintingPolicyProperty.CXPrintingPolicy_IncludeNewlines;
export const CXPrintingPolicy_MSVCFormatting: number = CXPrintingPolicyProperty.CXPrintingPolicy_MSVCFormatting;
export const CXPrintingPolicy_ConstantsAsWritten: number = CXPrintingPolicyProperty.CXPrintingPolicy_ConstantsAsWritten;
export const CXPrintingPolicy_SuppressImplicitBase: number = CXPrintingPolicyProperty.CXPrintingPolicy_SuppressImplicitBase;
export const CXPrintingPolicy_FullyQualifiedName: number = CXPrintingPolicyProperty.CXPrintingPolicy_FullyQualifiedName;
export const CXPrintingPolicy_LastProperty: number = CXPrintingPolicyProperty.CXPrintingPolicy_LastProperty;

/**
 * Property attributes for a \c CXCursor_ObjCPropertyDecl.
 */
export const CXObjCPropertyAttrKind = {
  CXObjCPropertyAttr_noattr: 0,
  CXObjCPropertyAttr_readonly: 1,
  CXObjCPropertyAttr_getter: 2,
  CXObjCPropertyAttr_assign: 4,
  CXObjCPropertyAttr_readwrite: 8,
  CXObjCPropertyAttr_retain: 16,
  CXObjCPropertyAttr_copy: 32,
  CXObjCPropertyAttr_nonatomic: 64,
  CXObjCPropertyAttr_setter: 128,
  CXObjCPropertyAttr_atomic: 256,
  CXObjCPropertyAttr_weak: 512,
  CXObjCPropertyAttr_strong: 1024,
  CXObjCPropertyAttr_unsafe_unretained: 2048,
  CXObjCPropertyAttr_class: 4096,
} as const;

export const CXObjCPropertyAttr_noattr: number = CXObjCPropertyAttrKind.CXObjCPropertyAttr_noattr;
export const CXObjCPropertyAttr_readonly: number = CXObjCPropertyAttrKind.CXObjCPropertyAttr_readonly;
export const CXObjCPropertyAttr_getter: number = CXObjCPropertyAttrKind.CXObjCPropertyAttr_getter;
export const CXObjCPropertyAttr_assign: number = CXObjCPropertyAttrKind.CXObjCPropertyAttr_assign;
export const CXObjCPropertyAttr_readwrite: number = CXObjCPropertyAttrKind.CXObjCPropertyAttr_readwrite;
export const CXObjCPropertyAttr_retain: number = CXObjCPropertyAttrKind.CXObjCPropertyAttr_retain;
export const CXObjCPropertyAttr_copy: number = CXObjCPropertyAttrKind.CXObjCPropertyAttr_copy;
export const CXObjCPropertyAttr_nonatomic: number = CXObjCPropertyAttrKind.CXObjCPropertyAttr_nonatomic;
export const CXObjCPropertyAttr_setter: number = CXObjCPropertyAttrKind.CXObjCPropertyAttr_setter;
export const CXObjCPropertyAttr_atomic: number = CXObjCPropertyAttrKind.CXObjCPropertyAttr_atomic;
export const CXObjCPropertyAttr_weak: number = CXObjCPropertyAttrKind.CXObjCPropertyAttr_weak;
export const CXObjCPropertyAttr_strong: number = CXObjCPropertyAttrKind.CXObjCPropertyAttr_strong;
export const CXObjCPropertyAttr_unsafe_unretained: number = CXObjCPropertyAttrKind.CXObjCPropertyAttr_unsafe_unretained;
export const CXObjCPropertyAttr_class: number = CXObjCPropertyAttrKind.CXObjCPropertyAttr_class;

/**
 * 'Qualifiers' written next to the return and parameter types in
Objective-C method declarations.
 */
export const CXObjCDeclQualifierKind = {
  CXObjCDeclQualifier_None: 0,
  CXObjCDeclQualifier_In: 1,
  CXObjCDeclQualifier_Inout: 2,
  CXObjCDeclQualifier_Out: 4,
  CXObjCDeclQualifier_Bycopy: 8,
  CXObjCDeclQualifier_Byref: 16,
  CXObjCDeclQualifier_Oneway: 32,
} as const;

export const CXObjCDeclQualifier_None: number = CXObjCDeclQualifierKind.CXObjCDeclQualifier_None;
export const CXObjCDeclQualifier_In: number = CXObjCDeclQualifierKind.CXObjCDeclQualifier_In;
export const CXObjCDeclQualifier_Inout: number = CXObjCDeclQualifierKind.CXObjCDeclQualifier_Inout;
export const CXObjCDeclQualifier_Out: number = CXObjCDeclQualifierKind.CXObjCDeclQualifier_Out;
export const CXObjCDeclQualifier_Bycopy: number = CXObjCDeclQualifierKind.CXObjCDeclQualifier_Bycopy;
export const CXObjCDeclQualifier_Byref: number = CXObjCDeclQualifierKind.CXObjCDeclQualifier_Byref;
export const CXObjCDeclQualifier_Oneway: number = CXObjCDeclQualifierKind.CXObjCDeclQualifier_Oneway;

export const CXNameRefFlags = {
  /** Include the nested-name-specifier, e.g. Foo:: in x.Foo::y, in the
range. */
  CXNameRange_WantQualifier: 1,
  /** Include the explicit template arguments, e.g. \<int> in x.f<int>,
in the range. */
  CXNameRange_WantTemplateArgs: 2,
  /** If the name is non-contiguous, return the full spanning range.

Non-contiguous names occur in Objective-C when a selector with two or more
parameters is used, or in C++ when using an operator:
\code
[object doSomething:here withValue:there]; // Objective-C
return some_vector[1]; // C++
\endcode */
  CXNameRange_WantSinglePiece: 4,
} as const;

/** Include the nested-name-specifier, e.g. Foo:: in x.Foo::y, in the
range. */
export const CXNameRange_WantQualifier: number = CXNameRefFlags.CXNameRange_WantQualifier;
/** Include the explicit template arguments, e.g. \<int> in x.f<int>,
in the range. */
export const CXNameRange_WantTemplateArgs: number = CXNameRefFlags.CXNameRange_WantTemplateArgs;
/** If the name is non-contiguous, return the full spanning range.

Non-contiguous names occur in Objective-C when a selector with two or more
parameters is used, or in C++ when using an operator:
\code
[object doSomething:here withValue:there]; // Objective-C
return some_vector[1]; // C++
\endcode */
export const CXNameRange_WantSinglePiece: number = CXNameRefFlags.CXNameRange_WantSinglePiece;

/**
 * Describes a kind of token.
 */
export const CXTokenKind = {
  /** A token that contains some kind of punctuation. */
  CXToken_Punctuation: 0,
  /** A language keyword. */
  CXToken_Keyword: 1,
  /** An identifier (that is not a keyword). */
  CXToken_Identifier: 2,
  /** A numeric, string, or character literal. */
  CXToken_Literal: 3,
  /** A comment. */
  CXToken_Comment: 4,
} as const;

/** A token that contains some kind of punctuation. */
export const CXToken_Punctuation: number = CXTokenKind.CXToken_Punctuation;
/** A language keyword. */
export const CXToken_Keyword: number = CXTokenKind.CXToken_Keyword;
/** An identifier (that is not a keyword). */
export const CXToken_Identifier: number = CXTokenKind.CXToken_Identifier;
/** A numeric, string, or character literal. */
export const CXToken_Literal: number = CXTokenKind.CXToken_Literal;
/** A comment. */
export const CXToken_Comment: number = CXTokenKind.CXToken_Comment;

/**
 * Describes a single piece of text within a code-completion string.

Each "chunk" within a code-completion string (\c CXCompletionString) is
either a piece of text with a specific "kind" that describes how that text
should be interpreted by the client or is another completion string.
 */
export const CXCompletionChunkKind = {
  /** A code-completion string that describes "optional" text that
could be a part of the template (but is not required).

The Optional chunk is the only kind of chunk that has a code-completion
string for its representation, which is accessible via
\c clang_getCompletionChunkCompletionString(). The code-completion string
describes an additional part of the template that is completely optional.
For example, optional chunks can be used to describe the placeholders for
arguments that match up with defaulted function parameters, e.g. given:

\code
void f(int x, float y = 3.14, double z = 2.71828);
\endcode

The code-completion string for this function would contain:
- a TypedText chunk for "f".
- a LeftParen chunk for "(".
- a Placeholder chunk for "int x"
- an Optional chunk containing the remaining defaulted arguments, e.g.,
- a Comma chunk for ","
- a Placeholder chunk for "float y"
- an Optional chunk containing the last defaulted argument:
- a Comma chunk for ","
- a Placeholder chunk for "double z"
- a RightParen chunk for ")"

There are many ways to handle Optional chunks. Two simple approaches are:
- Completely ignore optional chunks, in which case the template for the
function "f" would only include the first parameter ("int x").
- Fully expand all optional chunks, in which case the template for the
function "f" would have all of the parameters. */
  CXCompletionChunk_Optional: 0,
  /** Text that a user would be expected to type to get this
code-completion result.

There will be exactly one "typed text" chunk in a semantic string, which
will typically provide the spelling of a keyword or the name of a
declaration that could be used at the current code point. Clients are
expected to filter the code-completion results based on the text in this
chunk. */
  CXCompletionChunk_TypedText: 1,
  /** Text that should be inserted as part of a code-completion result.

A "text" chunk represents text that is part of the template to be
inserted into user code should this particular code-completion result
be selected. */
  CXCompletionChunk_Text: 2,
  /** Placeholder text that should be replaced by the user.

A "placeholder" chunk marks a place where the user should insert text
into the code-completion template. For example, placeholders might mark
the function parameters for a function declaration, to indicate that the
user should provide arguments for each of those parameters. The actual
text in a placeholder is a suggestion for the text to display before
the user replaces the placeholder with real code. */
  CXCompletionChunk_Placeholder: 3,
  /** Informative text that should be displayed but never inserted as
part of the template.

An "informative" chunk contains annotations that can be displayed to
help the user decide whether a particular code-completion result is the
right option, but which is not part of the actual template to be inserted
by code completion. */
  CXCompletionChunk_Informative: 4,
  /** Text that describes the current parameter when code-completion is
referring to function call, message send, or template specialization.

A "current parameter" chunk occurs when code-completion is providing
information about a parameter corresponding to the argument at the
code-completion point. For example, given a function

\code
int add(int x, int y);
\endcode

and the source code \c add(, where the code-completion point is after the
"(", the code-completion string will contain a "current parameter" chunk
for "int x", indicating that the current argument will initialize that
parameter. After typing further, to \c add(17, (where the code-completion
point is after the ","), the code-completion string will contain a
"current parameter" chunk to "int y". */
  CXCompletionChunk_CurrentParameter: 5,
  /** A left parenthesis ('('), used to initiate a function call or
signal the beginning of a function parameter list. */
  CXCompletionChunk_LeftParen: 6,
  /** A right parenthesis (')'), used to finish a function call or
signal the end of a function parameter list. */
  CXCompletionChunk_RightParen: 7,
  /** A left bracket ('['). */
  CXCompletionChunk_LeftBracket: 8,
  /** A right bracket (']'). */
  CXCompletionChunk_RightBracket: 9,
  /** A left brace ('{'). */
  CXCompletionChunk_LeftBrace: 10,
  /** A right brace ('}'). */
  CXCompletionChunk_RightBrace: 11,
  /** A left angle bracket ('<'). */
  CXCompletionChunk_LeftAngle: 12,
  /** A right angle bracket ('>'). */
  CXCompletionChunk_RightAngle: 13,
  /** A comma separator (','). */
  CXCompletionChunk_Comma: 14,
  /** Text that specifies the result type of a given result.

This special kind of informative chunk is not meant to be inserted into
the text buffer. Rather, it is meant to illustrate the type that an
expression using the given completion string would have. */
  CXCompletionChunk_ResultType: 15,
  /** A colon (':'). */
  CXCompletionChunk_Colon: 16,
  /** A semicolon (';'). */
  CXCompletionChunk_SemiColon: 17,
  /** An '=' sign. */
  CXCompletionChunk_Equal: 18,
  /** Horizontal space (' '). */
  CXCompletionChunk_HorizontalSpace: 19,
  /** Vertical space ('\\n'), after which it is generally a good idea to
perform indentation. */
  CXCompletionChunk_VerticalSpace: 20,
} as const;

/** A code-completion string that describes "optional" text that
could be a part of the template (but is not required).

The Optional chunk is the only kind of chunk that has a code-completion
string for its representation, which is accessible via
\c clang_getCompletionChunkCompletionString(). The code-completion string
describes an additional part of the template that is completely optional.
For example, optional chunks can be used to describe the placeholders for
arguments that match up with defaulted function parameters, e.g. given:

\code
void f(int x, float y = 3.14, double z = 2.71828);
\endcode

The code-completion string for this function would contain:
- a TypedText chunk for "f".
- a LeftParen chunk for "(".
- a Placeholder chunk for "int x"
- an Optional chunk containing the remaining defaulted arguments, e.g.,
- a Comma chunk for ","
- a Placeholder chunk for "float y"
- an Optional chunk containing the last defaulted argument:
- a Comma chunk for ","
- a Placeholder chunk for "double z"
- a RightParen chunk for ")"

There are many ways to handle Optional chunks. Two simple approaches are:
- Completely ignore optional chunks, in which case the template for the
function "f" would only include the first parameter ("int x").
- Fully expand all optional chunks, in which case the template for the
function "f" would have all of the parameters. */
export const CXCompletionChunk_Optional: number = CXCompletionChunkKind.CXCompletionChunk_Optional;
/** Text that a user would be expected to type to get this
code-completion result.

There will be exactly one "typed text" chunk in a semantic string, which
will typically provide the spelling of a keyword or the name of a
declaration that could be used at the current code point. Clients are
expected to filter the code-completion results based on the text in this
chunk. */
export const CXCompletionChunk_TypedText: number = CXCompletionChunkKind.CXCompletionChunk_TypedText;
/** Text that should be inserted as part of a code-completion result.

A "text" chunk represents text that is part of the template to be
inserted into user code should this particular code-completion result
be selected. */
export const CXCompletionChunk_Text: number = CXCompletionChunkKind.CXCompletionChunk_Text;
/** Placeholder text that should be replaced by the user.

A "placeholder" chunk marks a place where the user should insert text
into the code-completion template. For example, placeholders might mark
the function parameters for a function declaration, to indicate that the
user should provide arguments for each of those parameters. The actual
text in a placeholder is a suggestion for the text to display before
the user replaces the placeholder with real code. */
export const CXCompletionChunk_Placeholder: number = CXCompletionChunkKind.CXCompletionChunk_Placeholder;
/** Informative text that should be displayed but never inserted as
part of the template.

An "informative" chunk contains annotations that can be displayed to
help the user decide whether a particular code-completion result is the
right option, but which is not part of the actual template to be inserted
by code completion. */
export const CXCompletionChunk_Informative: number = CXCompletionChunkKind.CXCompletionChunk_Informative;
/** Text that describes the current parameter when code-completion is
referring to function call, message send, or template specialization.

A "current parameter" chunk occurs when code-completion is providing
information about a parameter corresponding to the argument at the
code-completion point. For example, given a function

\code
int add(int x, int y);
\endcode

and the source code \c add(, where the code-completion point is after the
"(", the code-completion string will contain a "current parameter" chunk
for "int x", indicating that the current argument will initialize that
parameter. After typing further, to \c add(17, (where the code-completion
point is after the ","), the code-completion string will contain a
"current parameter" chunk to "int y". */
export const CXCompletionChunk_CurrentParameter: number = CXCompletionChunkKind.CXCompletionChunk_CurrentParameter;
/** A left parenthesis ('('), used to initiate a function call or
signal the beginning of a function parameter list. */
export const CXCompletionChunk_LeftParen: number = CXCompletionChunkKind.CXCompletionChunk_LeftParen;
/** A right parenthesis (')'), used to finish a function call or
signal the end of a function parameter list. */
export const CXCompletionChunk_RightParen: number = CXCompletionChunkKind.CXCompletionChunk_RightParen;
/** A left bracket ('['). */
export const CXCompletionChunk_LeftBracket: number = CXCompletionChunkKind.CXCompletionChunk_LeftBracket;
/** A right bracket (']'). */
export const CXCompletionChunk_RightBracket: number = CXCompletionChunkKind.CXCompletionChunk_RightBracket;
/** A left brace ('{'). */
export const CXCompletionChunk_LeftBrace: number = CXCompletionChunkKind.CXCompletionChunk_LeftBrace;
/** A right brace ('}'). */
export const CXCompletionChunk_RightBrace: number = CXCompletionChunkKind.CXCompletionChunk_RightBrace;
/** A left angle bracket ('<'). */
export const CXCompletionChunk_LeftAngle: number = CXCompletionChunkKind.CXCompletionChunk_LeftAngle;
/** A right angle bracket ('>'). */
export const CXCompletionChunk_RightAngle: number = CXCompletionChunkKind.CXCompletionChunk_RightAngle;
/** A comma separator (','). */
export const CXCompletionChunk_Comma: number = CXCompletionChunkKind.CXCompletionChunk_Comma;
/** Text that specifies the result type of a given result.

This special kind of informative chunk is not meant to be inserted into
the text buffer. Rather, it is meant to illustrate the type that an
expression using the given completion string would have. */
export const CXCompletionChunk_ResultType: number = CXCompletionChunkKind.CXCompletionChunk_ResultType;
/** A colon (':'). */
export const CXCompletionChunk_Colon: number = CXCompletionChunkKind.CXCompletionChunk_Colon;
/** A semicolon (';'). */
export const CXCompletionChunk_SemiColon: number = CXCompletionChunkKind.CXCompletionChunk_SemiColon;
/** An '=' sign. */
export const CXCompletionChunk_Equal: number = CXCompletionChunkKind.CXCompletionChunk_Equal;
/** Horizontal space (' '). */
export const CXCompletionChunk_HorizontalSpace: number = CXCompletionChunkKind.CXCompletionChunk_HorizontalSpace;
/** Vertical space ('\\n'), after which it is generally a good idea to
perform indentation. */
export const CXCompletionChunk_VerticalSpace: number = CXCompletionChunkKind.CXCompletionChunk_VerticalSpace;

/**
 * Flags that can be passed to \c clang_codeCompleteAt() to
modify its behavior.

The enumerators in this enumeration can be bitwise-OR'd together to
provide multiple options to \c clang_codeCompleteAt().
 */
export const CXCodeComplete_Flags = {
  /** Whether to include macros within the set of code
completions returned. */
  CXCodeComplete_IncludeMacros: 1,
  /** Whether to include code patterns for language constructs
within the set of code completions, e.g., for loops. */
  CXCodeComplete_IncludeCodePatterns: 2,
  /** Whether to include brief documentation within the set of code
completions returned. */
  CXCodeComplete_IncludeBriefComments: 4,
  /** Whether to speed up completion by omitting top- or namespace-level entities
defined in the preamble. There's no guarantee any particular entity is
omitted. This may be useful if the headers are indexed externally. */
  CXCodeComplete_SkipPreamble: 8,
  /** Whether to include completions with small
fix-its, e.g. change '.' to '->' on member access, etc. */
  CXCodeComplete_IncludeCompletionsWithFixIts: 16,
} as const;

/** Whether to include macros within the set of code
completions returned. */
export const CXCodeComplete_IncludeMacros: number = CXCodeComplete_Flags.CXCodeComplete_IncludeMacros;
/** Whether to include code patterns for language constructs
within the set of code completions, e.g., for loops. */
export const CXCodeComplete_IncludeCodePatterns: number = CXCodeComplete_Flags.CXCodeComplete_IncludeCodePatterns;
/** Whether to include brief documentation within the set of code
completions returned. */
export const CXCodeComplete_IncludeBriefComments: number = CXCodeComplete_Flags.CXCodeComplete_IncludeBriefComments;
/** Whether to speed up completion by omitting top- or namespace-level entities
defined in the preamble. There's no guarantee any particular entity is
omitted. This may be useful if the headers are indexed externally. */
export const CXCodeComplete_SkipPreamble: number = CXCodeComplete_Flags.CXCodeComplete_SkipPreamble;
/** Whether to include completions with small
fix-its, e.g. change '.' to '->' on member access, etc. */
export const CXCodeComplete_IncludeCompletionsWithFixIts: number = CXCodeComplete_Flags.CXCodeComplete_IncludeCompletionsWithFixIts;

/**
 * Bits that represent the context under which completion is occurring.

The enumerators in this enumeration may be bitwise-OR'd together if multiple
contexts are occurring simultaneously.
 */
export const CXCompletionContext = {
  /** The context for completions is unexposed, as only Clang results
should be included. (This is equivalent to having no context bits set.) */
  CXCompletionContext_Unexposed: 0,
  /** Completions for any possible type should be included in the results. */
  CXCompletionContext_AnyType: 1,
  /** Completions for any possible value (variables, function calls, etc.)
should be included in the results. */
  CXCompletionContext_AnyValue: 2,
  /** Completions for values that resolve to an Objective-C object should
be included in the results. */
  CXCompletionContext_ObjCObjectValue: 4,
  /** Completions for values that resolve to an Objective-C selector
should be included in the results. */
  CXCompletionContext_ObjCSelectorValue: 8,
  /** Completions for values that resolve to a C++ class type should be
included in the results. */
  CXCompletionContext_CXXClassTypeValue: 16,
  /** Completions for fields of the member being accessed using the dot
operator should be included in the results. */
  CXCompletionContext_DotMemberAccess: 32,
  /** Completions for fields of the member being accessed using the arrow
operator should be included in the results. */
  CXCompletionContext_ArrowMemberAccess: 64,
  /** Completions for properties of the Objective-C object being accessed
using the dot operator should be included in the results. */
  CXCompletionContext_ObjCPropertyAccess: 128,
  /** Completions for enum tags should be included in the results. */
  CXCompletionContext_EnumTag: 256,
  /** Completions for union tags should be included in the results. */
  CXCompletionContext_UnionTag: 512,
  /** Completions for struct tags should be included in the results. */
  CXCompletionContext_StructTag: 1024,
  /** Completions for C++ class names should be included in the results. */
  CXCompletionContext_ClassTag: 2048,
  /** Completions for C++ namespaces and namespace aliases should be
included in the results. */
  CXCompletionContext_Namespace: 4096,
  /** Completions for C++ nested name specifiers should be included in
the results. */
  CXCompletionContext_NestedNameSpecifier: 8192,
  /** Completions for Objective-C interfaces (classes) should be included
in the results. */
  CXCompletionContext_ObjCInterface: 16384,
  /** Completions for Objective-C protocols should be included in
the results. */
  CXCompletionContext_ObjCProtocol: 32768,
  /** Completions for Objective-C categories should be included in
the results. */
  CXCompletionContext_ObjCCategory: 65536,
  /** Completions for Objective-C instance messages should be included
in the results. */
  CXCompletionContext_ObjCInstanceMessage: 131072,
  /** Completions for Objective-C class messages should be included in
the results. */
  CXCompletionContext_ObjCClassMessage: 262144,
  /** Completions for Objective-C selector names should be included in
the results. */
  CXCompletionContext_ObjCSelectorName: 524288,
  /** Completions for preprocessor macro names should be included in
the results. */
  CXCompletionContext_MacroName: 1048576,
  /** Natural language completions should be included in the results. */
  CXCompletionContext_NaturalLanguage: 2097152,
  /** #include file completions should be included in the results. */
  CXCompletionContext_IncludedFile: 4194304,
  /** The current context is unknown, so set all contexts. */
  CXCompletionContext_Unknown: 8388607,
} as const;

/** The context for completions is unexposed, as only Clang results
should be included. (This is equivalent to having no context bits set.) */
export const CXCompletionContext_Unexposed: number = CXCompletionContext.CXCompletionContext_Unexposed;
/** Completions for any possible type should be included in the results. */
export const CXCompletionContext_AnyType: number = CXCompletionContext.CXCompletionContext_AnyType;
/** Completions for any possible value (variables, function calls, etc.)
should be included in the results. */
export const CXCompletionContext_AnyValue: number = CXCompletionContext.CXCompletionContext_AnyValue;
/** Completions for values that resolve to an Objective-C object should
be included in the results. */
export const CXCompletionContext_ObjCObjectValue: number = CXCompletionContext.CXCompletionContext_ObjCObjectValue;
/** Completions for values that resolve to an Objective-C selector
should be included in the results. */
export const CXCompletionContext_ObjCSelectorValue: number = CXCompletionContext.CXCompletionContext_ObjCSelectorValue;
/** Completions for values that resolve to a C++ class type should be
included in the results. */
export const CXCompletionContext_CXXClassTypeValue: number = CXCompletionContext.CXCompletionContext_CXXClassTypeValue;
/** Completions for fields of the member being accessed using the dot
operator should be included in the results. */
export const CXCompletionContext_DotMemberAccess: number = CXCompletionContext.CXCompletionContext_DotMemberAccess;
/** Completions for fields of the member being accessed using the arrow
operator should be included in the results. */
export const CXCompletionContext_ArrowMemberAccess: number = CXCompletionContext.CXCompletionContext_ArrowMemberAccess;
/** Completions for properties of the Objective-C object being accessed
using the dot operator should be included in the results. */
export const CXCompletionContext_ObjCPropertyAccess: number = CXCompletionContext.CXCompletionContext_ObjCPropertyAccess;
/** Completions for enum tags should be included in the results. */
export const CXCompletionContext_EnumTag: number = CXCompletionContext.CXCompletionContext_EnumTag;
/** Completions for union tags should be included in the results. */
export const CXCompletionContext_UnionTag: number = CXCompletionContext.CXCompletionContext_UnionTag;
/** Completions for struct tags should be included in the results. */
export const CXCompletionContext_StructTag: number = CXCompletionContext.CXCompletionContext_StructTag;
/** Completions for C++ class names should be included in the results. */
export const CXCompletionContext_ClassTag: number = CXCompletionContext.CXCompletionContext_ClassTag;
/** Completions for C++ namespaces and namespace aliases should be
included in the results. */
export const CXCompletionContext_Namespace: number = CXCompletionContext.CXCompletionContext_Namespace;
/** Completions for C++ nested name specifiers should be included in
the results. */
export const CXCompletionContext_NestedNameSpecifier: number = CXCompletionContext.CXCompletionContext_NestedNameSpecifier;
/** Completions for Objective-C interfaces (classes) should be included
in the results. */
export const CXCompletionContext_ObjCInterface: number = CXCompletionContext.CXCompletionContext_ObjCInterface;
/** Completions for Objective-C protocols should be included in
the results. */
export const CXCompletionContext_ObjCProtocol: number = CXCompletionContext.CXCompletionContext_ObjCProtocol;
/** Completions for Objective-C categories should be included in
the results. */
export const CXCompletionContext_ObjCCategory: number = CXCompletionContext.CXCompletionContext_ObjCCategory;
/** Completions for Objective-C instance messages should be included
in the results. */
export const CXCompletionContext_ObjCInstanceMessage: number = CXCompletionContext.CXCompletionContext_ObjCInstanceMessage;
/** Completions for Objective-C class messages should be included in
the results. */
export const CXCompletionContext_ObjCClassMessage: number = CXCompletionContext.CXCompletionContext_ObjCClassMessage;
/** Completions for Objective-C selector names should be included in
the results. */
export const CXCompletionContext_ObjCSelectorName: number = CXCompletionContext.CXCompletionContext_ObjCSelectorName;
/** Completions for preprocessor macro names should be included in
the results. */
export const CXCompletionContext_MacroName: number = CXCompletionContext.CXCompletionContext_MacroName;
/** Natural language completions should be included in the results. */
export const CXCompletionContext_NaturalLanguage: number = CXCompletionContext.CXCompletionContext_NaturalLanguage;
/** #include file completions should be included in the results. */
export const CXCompletionContext_IncludedFile: number = CXCompletionContext.CXCompletionContext_IncludedFile;
/** The current context is unknown, so set all contexts. */
export const CXCompletionContext_Unknown: number = CXCompletionContext.CXCompletionContext_Unknown;

export const CXEvalResultKind = {
  CXEval_Int: 1,
  CXEval_Float: 2,
  CXEval_ObjCStrLiteral: 3,
  CXEval_StrLiteral: 4,
  CXEval_CFStr: 5,
  CXEval_Other: 6,
  CXEval_UnExposed: 0,
} as const;

export const CXEval_Int: number = CXEvalResultKind.CXEval_Int;
export const CXEval_Float: number = CXEvalResultKind.CXEval_Float;
export const CXEval_ObjCStrLiteral: number = CXEvalResultKind.CXEval_ObjCStrLiteral;
export const CXEval_StrLiteral: number = CXEvalResultKind.CXEval_StrLiteral;
export const CXEval_CFStr: number = CXEvalResultKind.CXEval_CFStr;
export const CXEval_Other: number = CXEvalResultKind.CXEval_Other;
export const CXEval_UnExposed: number = CXEvalResultKind.CXEval_UnExposed;

/**
 * \defgroup CINDEX_HIGH Higher level API functions

@{
 */
export const CXVisitorResult = {
  CXVisit_Break: 0,
  CXVisit_Continue: 1,
} as const;

export const CXVisit_Break: number = CXVisitorResult.CXVisit_Break;
export const CXVisit_Continue: number = CXVisitorResult.CXVisit_Continue;

export const CXResult = {
  /** Function returned successfully. */
  CXResult_Success: 0,
  /** One of the parameters was invalid for the function. */
  CXResult_Invalid: 1,
  /** The function was terminated by a callback (e.g. it returned
CXVisit_Break) */
  CXResult_VisitBreak: 2,
} as const;

/** Function returned successfully. */
export const CXResult_Success: number = CXResult.CXResult_Success;
/** One of the parameters was invalid for the function. */
export const CXResult_Invalid: number = CXResult.CXResult_Invalid;
/** The function was terminated by a callback (e.g. it returned
CXVisit_Break) */
export const CXResult_VisitBreak: number = CXResult.CXResult_VisitBreak;

export const CXIdxEntityKind = {
  CXIdxEntity_Unexposed: 0,
  CXIdxEntity_Typedef: 1,
  CXIdxEntity_Function: 2,
  CXIdxEntity_Variable: 3,
  CXIdxEntity_Field: 4,
  CXIdxEntity_EnumConstant: 5,
  CXIdxEntity_ObjCClass: 6,
  CXIdxEntity_ObjCProtocol: 7,
  CXIdxEntity_ObjCCategory: 8,
  CXIdxEntity_ObjCInstanceMethod: 9,
  CXIdxEntity_ObjCClassMethod: 10,
  CXIdxEntity_ObjCProperty: 11,
  CXIdxEntity_ObjCIvar: 12,
  CXIdxEntity_Enum: 13,
  CXIdxEntity_Struct: 14,
  CXIdxEntity_Union: 15,
  CXIdxEntity_CXXClass: 16,
  CXIdxEntity_CXXNamespace: 17,
  CXIdxEntity_CXXNamespaceAlias: 18,
  CXIdxEntity_CXXStaticVariable: 19,
  CXIdxEntity_CXXStaticMethod: 20,
  CXIdxEntity_CXXInstanceMethod: 21,
  CXIdxEntity_CXXConstructor: 22,
  CXIdxEntity_CXXDestructor: 23,
  CXIdxEntity_CXXConversionFunction: 24,
  CXIdxEntity_CXXTypeAlias: 25,
  CXIdxEntity_CXXInterface: 26,
  CXIdxEntity_CXXConcept: 27,
} as const;

export const CXIdxEntity_Unexposed: number = CXIdxEntityKind.CXIdxEntity_Unexposed;
export const CXIdxEntity_Typedef: number = CXIdxEntityKind.CXIdxEntity_Typedef;
export const CXIdxEntity_Function: number = CXIdxEntityKind.CXIdxEntity_Function;
export const CXIdxEntity_Variable: number = CXIdxEntityKind.CXIdxEntity_Variable;
export const CXIdxEntity_Field: number = CXIdxEntityKind.CXIdxEntity_Field;
export const CXIdxEntity_EnumConstant: number = CXIdxEntityKind.CXIdxEntity_EnumConstant;
export const CXIdxEntity_ObjCClass: number = CXIdxEntityKind.CXIdxEntity_ObjCClass;
export const CXIdxEntity_ObjCProtocol: number = CXIdxEntityKind.CXIdxEntity_ObjCProtocol;
export const CXIdxEntity_ObjCCategory: number = CXIdxEntityKind.CXIdxEntity_ObjCCategory;
export const CXIdxEntity_ObjCInstanceMethod: number = CXIdxEntityKind.CXIdxEntity_ObjCInstanceMethod;
export const CXIdxEntity_ObjCClassMethod: number = CXIdxEntityKind.CXIdxEntity_ObjCClassMethod;
export const CXIdxEntity_ObjCProperty: number = CXIdxEntityKind.CXIdxEntity_ObjCProperty;
export const CXIdxEntity_ObjCIvar: number = CXIdxEntityKind.CXIdxEntity_ObjCIvar;
export const CXIdxEntity_Enum: number = CXIdxEntityKind.CXIdxEntity_Enum;
export const CXIdxEntity_Struct: number = CXIdxEntityKind.CXIdxEntity_Struct;
export const CXIdxEntity_Union: number = CXIdxEntityKind.CXIdxEntity_Union;
export const CXIdxEntity_CXXClass: number = CXIdxEntityKind.CXIdxEntity_CXXClass;
export const CXIdxEntity_CXXNamespace: number = CXIdxEntityKind.CXIdxEntity_CXXNamespace;
export const CXIdxEntity_CXXNamespaceAlias: number = CXIdxEntityKind.CXIdxEntity_CXXNamespaceAlias;
export const CXIdxEntity_CXXStaticVariable: number = CXIdxEntityKind.CXIdxEntity_CXXStaticVariable;
export const CXIdxEntity_CXXStaticMethod: number = CXIdxEntityKind.CXIdxEntity_CXXStaticMethod;
export const CXIdxEntity_CXXInstanceMethod: number = CXIdxEntityKind.CXIdxEntity_CXXInstanceMethod;
export const CXIdxEntity_CXXConstructor: number = CXIdxEntityKind.CXIdxEntity_CXXConstructor;
export const CXIdxEntity_CXXDestructor: number = CXIdxEntityKind.CXIdxEntity_CXXDestructor;
export const CXIdxEntity_CXXConversionFunction: number = CXIdxEntityKind.CXIdxEntity_CXXConversionFunction;
export const CXIdxEntity_CXXTypeAlias: number = CXIdxEntityKind.CXIdxEntity_CXXTypeAlias;
export const CXIdxEntity_CXXInterface: number = CXIdxEntityKind.CXIdxEntity_CXXInterface;
export const CXIdxEntity_CXXConcept: number = CXIdxEntityKind.CXIdxEntity_CXXConcept;

export const CXIdxEntityLanguage = {
  CXIdxEntityLang_None: 0,
  CXIdxEntityLang_C: 1,
  CXIdxEntityLang_ObjC: 2,
  CXIdxEntityLang_CXX: 3,
  CXIdxEntityLang_Swift: 4,
} as const;

export const CXIdxEntityLang_None: number = CXIdxEntityLanguage.CXIdxEntityLang_None;
export const CXIdxEntityLang_C: number = CXIdxEntityLanguage.CXIdxEntityLang_C;
export const CXIdxEntityLang_ObjC: number = CXIdxEntityLanguage.CXIdxEntityLang_ObjC;
export const CXIdxEntityLang_CXX: number = CXIdxEntityLanguage.CXIdxEntityLang_CXX;
export const CXIdxEntityLang_Swift: number = CXIdxEntityLanguage.CXIdxEntityLang_Swift;

/**
 * Extra C++ template information for an entity. This can apply to:
CXIdxEntity_Function
CXIdxEntity_CXXClass
CXIdxEntity_CXXStaticMethod
CXIdxEntity_CXXInstanceMethod
CXIdxEntity_CXXConstructor
CXIdxEntity_CXXConversionFunction
CXIdxEntity_CXXTypeAlias
 */
export const CXIdxEntityCXXTemplateKind = {
  CXIdxEntity_NonTemplate: 0,
  CXIdxEntity_Template: 1,
  CXIdxEntity_TemplatePartialSpecialization: 2,
  CXIdxEntity_TemplateSpecialization: 3,
} as const;

export const CXIdxEntity_NonTemplate: number = CXIdxEntityCXXTemplateKind.CXIdxEntity_NonTemplate;
export const CXIdxEntity_Template: number = CXIdxEntityCXXTemplateKind.CXIdxEntity_Template;
export const CXIdxEntity_TemplatePartialSpecialization: number = CXIdxEntityCXXTemplateKind.CXIdxEntity_TemplatePartialSpecialization;
export const CXIdxEntity_TemplateSpecialization: number = CXIdxEntityCXXTemplateKind.CXIdxEntity_TemplateSpecialization;

export const CXIdxAttrKind = {
  CXIdxAttr_Unexposed: 0,
  CXIdxAttr_IBAction: 1,
  CXIdxAttr_IBOutlet: 2,
  CXIdxAttr_IBOutletCollection: 3,
} as const;

export const CXIdxAttr_Unexposed: number = CXIdxAttrKind.CXIdxAttr_Unexposed;
export const CXIdxAttr_IBAction: number = CXIdxAttrKind.CXIdxAttr_IBAction;
export const CXIdxAttr_IBOutlet: number = CXIdxAttrKind.CXIdxAttr_IBOutlet;
export const CXIdxAttr_IBOutletCollection: number = CXIdxAttrKind.CXIdxAttr_IBOutletCollection;

export const CXIdxDeclInfoFlags = {
  CXIdxDeclFlag_Skipped: 1,
} as const;

export const CXIdxDeclFlag_Skipped: number = CXIdxDeclInfoFlags.CXIdxDeclFlag_Skipped;

export const CXIdxObjCContainerKind = {
  CXIdxObjCContainer_ForwardRef: 0,
  CXIdxObjCContainer_Interface: 1,
  CXIdxObjCContainer_Implementation: 2,
} as const;

export const CXIdxObjCContainer_ForwardRef: number = CXIdxObjCContainerKind.CXIdxObjCContainer_ForwardRef;
export const CXIdxObjCContainer_Interface: number = CXIdxObjCContainerKind.CXIdxObjCContainer_Interface;
export const CXIdxObjCContainer_Implementation: number = CXIdxObjCContainerKind.CXIdxObjCContainer_Implementation;

/**
 * Data for IndexerCallbacks#indexEntityReference.

This may be deprecated in a future version as this duplicates
the \c CXSymbolRole_Implicit bit in \c CXSymbolRole.
 */
export const CXIdxEntityRefKind = {
  /** The entity is referenced directly in user's code. */
  CXIdxEntityRef_Direct: 1,
  /** An implicit reference, e.g. a reference of an Objective-C method
via the dot syntax. */
  CXIdxEntityRef_Implicit: 2,
} as const;

/** The entity is referenced directly in user's code. */
export const CXIdxEntityRef_Direct: number = CXIdxEntityRefKind.CXIdxEntityRef_Direct;
/** An implicit reference, e.g. a reference of an Objective-C method
via the dot syntax. */
export const CXIdxEntityRef_Implicit: number = CXIdxEntityRefKind.CXIdxEntityRef_Implicit;

/**
 * Roles that are attributed to symbol occurrences.

Internal: this currently mirrors low 9 bits of clang::index::SymbolRole with
higher bits zeroed. These high bits may be exposed in the future.
 */
export const CXSymbolRole = {
  CXSymbolRole_None: 0,
  CXSymbolRole_Declaration: 1,
  CXSymbolRole_Definition: 2,
  CXSymbolRole_Reference: 4,
  CXSymbolRole_Read: 8,
  CXSymbolRole_Write: 16,
  CXSymbolRole_Call: 32,
  CXSymbolRole_Dynamic: 64,
  CXSymbolRole_AddressOf: 128,
  CXSymbolRole_Implicit: 256,
} as const;

export const CXSymbolRole_None: number = CXSymbolRole.CXSymbolRole_None;
export const CXSymbolRole_Declaration: number = CXSymbolRole.CXSymbolRole_Declaration;
export const CXSymbolRole_Definition: number = CXSymbolRole.CXSymbolRole_Definition;
export const CXSymbolRole_Reference: number = CXSymbolRole.CXSymbolRole_Reference;
export const CXSymbolRole_Read: number = CXSymbolRole.CXSymbolRole_Read;
export const CXSymbolRole_Write: number = CXSymbolRole.CXSymbolRole_Write;
export const CXSymbolRole_Call: number = CXSymbolRole.CXSymbolRole_Call;
export const CXSymbolRole_Dynamic: number = CXSymbolRole.CXSymbolRole_Dynamic;
export const CXSymbolRole_AddressOf: number = CXSymbolRole.CXSymbolRole_AddressOf;
export const CXSymbolRole_Implicit: number = CXSymbolRole.CXSymbolRole_Implicit;

export const CXIndexOptFlags = {
  /** Used to indicate that no special indexing options are needed. */
  CXIndexOpt_None: 0,
  /** Used to indicate that IndexerCallbacks#indexEntityReference should
be invoked for only one reference of an entity per source file that does
not also include a declaration/definition of the entity. */
  CXIndexOpt_SuppressRedundantRefs: 1,
  /** Function-local symbols should be indexed. If this is not set
function-local symbols will be ignored. */
  CXIndexOpt_IndexFunctionLocalSymbols: 2,
  /** Implicit function/class template instantiations should be indexed.
If this is not set, implicit instantiations will be ignored. */
  CXIndexOpt_IndexImplicitTemplateInstantiations: 4,
  /** Suppress all compiler warnings when parsing for indexing. */
  CXIndexOpt_SuppressWarnings: 8,
  /** Skip a function/method body that was already parsed during an
indexing session associated with a \c CXIndexAction object.
Bodies in system headers are always skipped. */
  CXIndexOpt_SkipParsedBodiesInSession: 16,
} as const;

/** Used to indicate that no special indexing options are needed. */
export const CXIndexOpt_None: number = CXIndexOptFlags.CXIndexOpt_None;
/** Used to indicate that IndexerCallbacks#indexEntityReference should
be invoked for only one reference of an entity per source file that does
not also include a declaration/definition of the entity. */
export const CXIndexOpt_SuppressRedundantRefs: number = CXIndexOptFlags.CXIndexOpt_SuppressRedundantRefs;
/** Function-local symbols should be indexed. If this is not set
function-local symbols will be ignored. */
export const CXIndexOpt_IndexFunctionLocalSymbols: number = CXIndexOptFlags.CXIndexOpt_IndexFunctionLocalSymbols;
/** Implicit function/class template instantiations should be indexed.
If this is not set, implicit instantiations will be ignored. */
export const CXIndexOpt_IndexImplicitTemplateInstantiations: number = CXIndexOptFlags.CXIndexOpt_IndexImplicitTemplateInstantiations;
/** Suppress all compiler warnings when parsing for indexing. */
export const CXIndexOpt_SuppressWarnings: number = CXIndexOptFlags.CXIndexOpt_SuppressWarnings;
/** Skip a function/method body that was already parsed during an
indexing session associated with a \c CXIndexAction object.
Bodies in system headers are always skipped. */
export const CXIndexOpt_SkipParsedBodiesInSession: number = CXIndexOptFlags.CXIndexOpt_SkipParsedBodiesInSession;

/**
 * Describes the kind of binary operators.
 */
export const CXBinaryOperatorKind = {
  /** This value describes cursors which are not binary operators. */
  CXBinaryOperator_Invalid: 0,
  /** C++ Pointer - to - member operator. */
  CXBinaryOperator_PtrMemD: 1,
  /** C++ Pointer - to - member operator. */
  CXBinaryOperator_PtrMemI: 2,
  /** Multiplication operator. */
  CXBinaryOperator_Mul: 3,
  /** Division operator. */
  CXBinaryOperator_Div: 4,
  /** Remainder operator. */
  CXBinaryOperator_Rem: 5,
  /** Addition operator. */
  CXBinaryOperator_Add: 6,
  /** Subtraction operator. */
  CXBinaryOperator_Sub: 7,
  /** Bitwise shift left operator. */
  CXBinaryOperator_Shl: 8,
  /** Bitwise shift right operator. */
  CXBinaryOperator_Shr: 9,
  /** C++ three-way comparison (spaceship) operator. */
  CXBinaryOperator_Cmp: 10,
  /** Less than operator. */
  CXBinaryOperator_LT: 11,
  /** Greater than operator. */
  CXBinaryOperator_GT: 12,
  /** Less or equal operator. */
  CXBinaryOperator_LE: 13,
  /** Greater or equal operator. */
  CXBinaryOperator_GE: 14,
  /** Equal operator. */
  CXBinaryOperator_EQ: 15,
  /** Not equal operator. */
  CXBinaryOperator_NE: 16,
  /** Bitwise AND operator. */
  CXBinaryOperator_And: 17,
  /** Bitwise XOR operator. */
  CXBinaryOperator_Xor: 18,
  /** Bitwise OR operator. */
  CXBinaryOperator_Or: 19,
  /** Logical AND operator. */
  CXBinaryOperator_LAnd: 20,
  /** Logical OR operator. */
  CXBinaryOperator_LOr: 21,
  /** Assignment operator. */
  CXBinaryOperator_Assign: 22,
  /** Multiplication assignment operator. */
  CXBinaryOperator_MulAssign: 23,
  /** Division assignment operator. */
  CXBinaryOperator_DivAssign: 24,
  /** Remainder assignment operator. */
  CXBinaryOperator_RemAssign: 25,
  /** Addition assignment operator. */
  CXBinaryOperator_AddAssign: 26,
  /** Subtraction assignment operator. */
  CXBinaryOperator_SubAssign: 27,
  /** Bitwise shift left assignment operator. */
  CXBinaryOperator_ShlAssign: 28,
  /** Bitwise shift right assignment operator. */
  CXBinaryOperator_ShrAssign: 29,
  /** Bitwise AND assignment operator. */
  CXBinaryOperator_AndAssign: 30,
  /** Bitwise XOR assignment operator. */
  CXBinaryOperator_XorAssign: 31,
  /** Bitwise OR assignment operator. */
  CXBinaryOperator_OrAssign: 32,
  /** Comma operator. */
  CXBinaryOperator_Comma: 33,
} as const;

/** This value describes cursors which are not binary operators. */
export const CXBinaryOperator_Invalid: number = CXBinaryOperatorKind.CXBinaryOperator_Invalid;
/** C++ Pointer - to - member operator. */
export const CXBinaryOperator_PtrMemD: number = CXBinaryOperatorKind.CXBinaryOperator_PtrMemD;
/** C++ Pointer - to - member operator. */
export const CXBinaryOperator_PtrMemI: number = CXBinaryOperatorKind.CXBinaryOperator_PtrMemI;
/** Multiplication operator. */
export const CXBinaryOperator_Mul: number = CXBinaryOperatorKind.CXBinaryOperator_Mul;
/** Division operator. */
export const CXBinaryOperator_Div: number = CXBinaryOperatorKind.CXBinaryOperator_Div;
/** Remainder operator. */
export const CXBinaryOperator_Rem: number = CXBinaryOperatorKind.CXBinaryOperator_Rem;
/** Addition operator. */
export const CXBinaryOperator_Add: number = CXBinaryOperatorKind.CXBinaryOperator_Add;
/** Subtraction operator. */
export const CXBinaryOperator_Sub: number = CXBinaryOperatorKind.CXBinaryOperator_Sub;
/** Bitwise shift left operator. */
export const CXBinaryOperator_Shl: number = CXBinaryOperatorKind.CXBinaryOperator_Shl;
/** Bitwise shift right operator. */
export const CXBinaryOperator_Shr: number = CXBinaryOperatorKind.CXBinaryOperator_Shr;
/** C++ three-way comparison (spaceship) operator. */
export const CXBinaryOperator_Cmp: number = CXBinaryOperatorKind.CXBinaryOperator_Cmp;
/** Less than operator. */
export const CXBinaryOperator_LT: number = CXBinaryOperatorKind.CXBinaryOperator_LT;
/** Greater than operator. */
export const CXBinaryOperator_GT: number = CXBinaryOperatorKind.CXBinaryOperator_GT;
/** Less or equal operator. */
export const CXBinaryOperator_LE: number = CXBinaryOperatorKind.CXBinaryOperator_LE;
/** Greater or equal operator. */
export const CXBinaryOperator_GE: number = CXBinaryOperatorKind.CXBinaryOperator_GE;
/** Equal operator. */
export const CXBinaryOperator_EQ: number = CXBinaryOperatorKind.CXBinaryOperator_EQ;
/** Not equal operator. */
export const CXBinaryOperator_NE: number = CXBinaryOperatorKind.CXBinaryOperator_NE;
/** Bitwise AND operator. */
export const CXBinaryOperator_And: number = CXBinaryOperatorKind.CXBinaryOperator_And;
/** Bitwise XOR operator. */
export const CXBinaryOperator_Xor: number = CXBinaryOperatorKind.CXBinaryOperator_Xor;
/** Bitwise OR operator. */
export const CXBinaryOperator_Or: number = CXBinaryOperatorKind.CXBinaryOperator_Or;
/** Logical AND operator. */
export const CXBinaryOperator_LAnd: number = CXBinaryOperatorKind.CXBinaryOperator_LAnd;
/** Logical OR operator. */
export const CXBinaryOperator_LOr: number = CXBinaryOperatorKind.CXBinaryOperator_LOr;
/** Assignment operator. */
export const CXBinaryOperator_Assign: number = CXBinaryOperatorKind.CXBinaryOperator_Assign;
/** Multiplication assignment operator. */
export const CXBinaryOperator_MulAssign: number = CXBinaryOperatorKind.CXBinaryOperator_MulAssign;
/** Division assignment operator. */
export const CXBinaryOperator_DivAssign: number = CXBinaryOperatorKind.CXBinaryOperator_DivAssign;
/** Remainder assignment operator. */
export const CXBinaryOperator_RemAssign: number = CXBinaryOperatorKind.CXBinaryOperator_RemAssign;
/** Addition assignment operator. */
export const CXBinaryOperator_AddAssign: number = CXBinaryOperatorKind.CXBinaryOperator_AddAssign;
/** Subtraction assignment operator. */
export const CXBinaryOperator_SubAssign: number = CXBinaryOperatorKind.CXBinaryOperator_SubAssign;
/** Bitwise shift left assignment operator. */
export const CXBinaryOperator_ShlAssign: number = CXBinaryOperatorKind.CXBinaryOperator_ShlAssign;
/** Bitwise shift right assignment operator. */
export const CXBinaryOperator_ShrAssign: number = CXBinaryOperatorKind.CXBinaryOperator_ShrAssign;
/** Bitwise AND assignment operator. */
export const CXBinaryOperator_AndAssign: number = CXBinaryOperatorKind.CXBinaryOperator_AndAssign;
/** Bitwise XOR assignment operator. */
export const CXBinaryOperator_XorAssign: number = CXBinaryOperatorKind.CXBinaryOperator_XorAssign;
/** Bitwise OR assignment operator. */
export const CXBinaryOperator_OrAssign: number = CXBinaryOperatorKind.CXBinaryOperator_OrAssign;
/** Comma operator. */
export const CXBinaryOperator_Comma: number = CXBinaryOperatorKind.CXBinaryOperator_Comma;

/**
 * Describes the kind of unary operators.
 */
export const CXUnaryOperatorKind = {
  /** This value describes cursors which are not unary operators. */
  CXUnaryOperator_Invalid: 0,
  /** Postfix increment operator. */
  CXUnaryOperator_PostInc: 1,
  /** Postfix decrement operator. */
  CXUnaryOperator_PostDec: 2,
  /** Prefix increment operator. */
  CXUnaryOperator_PreInc: 3,
  /** Prefix decrement operator. */
  CXUnaryOperator_PreDec: 4,
  /** Address of operator. */
  CXUnaryOperator_AddrOf: 5,
  /** Dereference operator. */
  CXUnaryOperator_Deref: 6,
  /** Plus operator. */
  CXUnaryOperator_Plus: 7,
  /** Minus operator. */
  CXUnaryOperator_Minus: 8,
  /** Not operator. */
  CXUnaryOperator_Not: 9,
  /** LNot operator. */
  CXUnaryOperator_LNot: 10,
  /** "__real expr" operator. */
  CXUnaryOperator_Real: 11,
  /** "__imag expr" operator. */
  CXUnaryOperator_Imag: 12,
  /** __extension__ marker operator. */
  CXUnaryOperator_Extension: 13,
  /** C++ co_await operator. */
  CXUnaryOperator_Coawait: 14,
} as const;

/** This value describes cursors which are not unary operators. */
export const CXUnaryOperator_Invalid: number = CXUnaryOperatorKind.CXUnaryOperator_Invalid;
/** Postfix increment operator. */
export const CXUnaryOperator_PostInc: number = CXUnaryOperatorKind.CXUnaryOperator_PostInc;
/** Postfix decrement operator. */
export const CXUnaryOperator_PostDec: number = CXUnaryOperatorKind.CXUnaryOperator_PostDec;
/** Prefix increment operator. */
export const CXUnaryOperator_PreInc: number = CXUnaryOperatorKind.CXUnaryOperator_PreInc;
/** Prefix decrement operator. */
export const CXUnaryOperator_PreDec: number = CXUnaryOperatorKind.CXUnaryOperator_PreDec;
/** Address of operator. */
export const CXUnaryOperator_AddrOf: number = CXUnaryOperatorKind.CXUnaryOperator_AddrOf;
/** Dereference operator. */
export const CXUnaryOperator_Deref: number = CXUnaryOperatorKind.CXUnaryOperator_Deref;
/** Plus operator. */
export const CXUnaryOperator_Plus: number = CXUnaryOperatorKind.CXUnaryOperator_Plus;
/** Minus operator. */
export const CXUnaryOperator_Minus: number = CXUnaryOperatorKind.CXUnaryOperator_Minus;
/** Not operator. */
export const CXUnaryOperator_Not: number = CXUnaryOperatorKind.CXUnaryOperator_Not;
/** LNot operator. */
export const CXUnaryOperator_LNot: number = CXUnaryOperatorKind.CXUnaryOperator_LNot;
/** "__real expr" operator. */
export const CXUnaryOperator_Real: number = CXUnaryOperatorKind.CXUnaryOperator_Real;
/** "__imag expr" operator. */
export const CXUnaryOperator_Imag: number = CXUnaryOperatorKind.CXUnaryOperator_Imag;
/** __extension__ marker operator. */
export const CXUnaryOperator_Extension: number = CXUnaryOperatorKind.CXUnaryOperator_Extension;
/** C++ co_await operator. */
export const CXUnaryOperator_Coawait: number = CXUnaryOperatorKind.CXUnaryOperator_Coawait;


// Function exports
/**
 * Provides a shared context for creating translation units.

It provides two options:

- excludeDeclarationsFromPCH: When non-zero, allows enumeration of "local"
declarations (when loading any new translation units). A "local" declaration
is one that belongs in the translation unit itself and not in a precompiled
header that was used by the translation unit. If zero, all declarations
will be enumerated.

Here is an example:

\code
// excludeDeclsFromPCH = 1, displayDiagnostics=1
Idx = clang_createIndex(1, 1);

// IndexTest.pch was produced with the following command:
// "clang -x c IndexTest.h -emit-ast -o IndexTest.pch"
TU = clang_createTranslationUnit(Idx, "IndexTest.pch");

// This will load all the symbols from 'IndexTest.pch'
clang_visitChildren(clang_getTranslationUnitCursor(TU),
TranslationUnitVisitor, 0);
clang_disposeTranslationUnit(TU);

// This will load all the symbols from 'IndexTest.c', excluding symbols
// from 'IndexTest.pch'.
char *args[] = { "-Xclang", "-include-pch=IndexTest.pch" };
TU = clang_createTranslationUnitFromSourceFile(Idx, "IndexTest.c", 2, args,
0, 0);
clang_visitChildren(clang_getTranslationUnitCursor(TU),
TranslationUnitVisitor, 0);
clang_disposeTranslationUnit(TU);
\endcode

This process of creating the 'pch', loading it separately, and using it (via
-include-pch) allows 'excludeDeclsFromPCH' to remove redundant callbacks
(which gives the indexer the same performance benefit as the compiler).
 * @param excludeDeclarationsFromPCH - int
 * @param displayDiagnostics - int
 * @returns CXIndex
 */
export function clang_createIndex(excludeDeclarationsFromPCH: number, displayDiagnostics: number): { _type: 'CXIndex' } {
  return addon.clang_createIndex(excludeDeclarationsFromPCH, displayDiagnostics);
}

/**
 * Destroy the given index.

The index must not be destroyed until all of the translation units created
within that index have been destroyed.
 * @param index - CXIndex
 */
export function clang_disposeIndex(index: { _type: 'CXIndex' }): void {
  addon.clang_disposeIndex(index);
}

/**
 * Provides a shared context for creating translation units.

Call this function instead of clang_createIndex() if you need to configure
the additional options in CXIndexOptions.

\returns The created index or null in case of error, such as an unsupported
value of options->Size.

For example:
\code
CXIndex createIndex(const char *ApplicationTemporaryPath) {
const int ExcludeDeclarationsFromPCH = 1;
const int DisplayDiagnostics = 1;
CXIndex Idx;
#if CINDEX_VERSION_MINOR >= 64
CXIndexOptions Opts;
memset(&Opts, 0, sizeof(Opts));
Opts.Size = sizeof(CXIndexOptions);
Opts.ThreadBackgroundPriorityForIndexing = 1;
Opts.ExcludeDeclarationsFromPCH = ExcludeDeclarationsFromPCH;
Opts.DisplayDiagnostics = DisplayDiagnostics;
Opts.PreambleStoragePath = ApplicationTemporaryPath;
Idx = clang_createIndexWithOptions(&Opts);
if (Idx)
return Idx;
fprintf(stderr,
"clang_createIndexWithOptions() failed. "
"CINDEX_VERSION_MINOR = %d, sizeof(CXIndexOptions) = %u\n",
CINDEX_VERSION_MINOR, Opts.Size);
#else
(void)ApplicationTemporaryPath;
#endif
Idx = clang_createIndex(ExcludeDeclarationsFromPCH, DisplayDiagnostics);
clang_CXIndex_setGlobalOptions(
Idx, clang_CXIndex_getGlobalOptions(Idx) |
CXGlobalOpt_ThreadBackgroundPriorityForIndexing);
return Idx;
}
\endcode

\sa clang_createIndex()
 * @param options - const CXIndexOptions *
 * @returns CXIndex
 */
export function clang_createIndexWithOptions(options: { _ptr: unknown }): { _type: 'CXIndex' } {
  return addon.clang_createIndexWithOptions(options);
}

/**
 * Sets general options associated with a CXIndex.

This function is DEPRECATED. Set
CXIndexOptions::ThreadBackgroundPriorityForIndexing and/or
CXIndexOptions::ThreadBackgroundPriorityForEditing and call
clang_createIndexWithOptions() instead.

For example:
\code
CXIndex idx = ...;
clang_CXIndex_setGlobalOptions(idx,
clang_CXIndex_getGlobalOptions(idx) |
CXGlobalOpt_ThreadBackgroundPriorityForIndexing);
\endcode

\param options A bitmask of options, a bitwise OR of CXGlobalOpt_XXX flags.
 * @param arg - CXIndex
 * @param options - unsigned int
 */
export function clang_CXIndex_setGlobalOptions(arg0: { _type: 'CXIndex' }, options: number): void {
  addon.clang_CXIndex_setGlobalOptions(arg0, options);
}

/**
 * Gets the general options associated with a CXIndex.

This function allows to obtain the final option values used by libclang after
specifying the option policies via CXChoice enumerators.

\returns A bitmask of options, a bitwise OR of CXGlobalOpt_XXX flags that
are associated with the given CXIndex object.
 * @param arg - CXIndex
 * @returns unsigned int
 */
export function clang_CXIndex_getGlobalOptions(arg0: { _type: 'CXIndex' }): number {
  return addon.clang_CXIndex_getGlobalOptions(arg0);
}

/**
 * Sets the invocation emission path option in a CXIndex.

This function is DEPRECATED. Set CXIndexOptions::InvocationEmissionPath and
call clang_createIndexWithOptions() instead.

The invocation emission path specifies a path which will contain log
files for certain libclang invocations. A null value (default) implies that
libclang invocations are not logged..
 * @param arg - CXIndex
 * @param Path - const char *
 */
export function clang_CXIndex_setInvocationEmissionPathOption(arg0: { _type: 'CXIndex' }, Path: string): void {
  addon.clang_CXIndex_setInvocationEmissionPathOption(arg0, Path);
}

/**
 * Determine whether the given header is guarded against
multiple inclusions, either with the conventional
\#ifndef/\#define/\#endif macro guards or with \#pragma once.
 * @param tu - CXTranslationUnit
 * @param file - CXFile
 * @returns unsigned int
 */
export function clang_isFileMultipleIncludeGuarded(tu: { _type: 'CXTranslationUnit' }, file: { _type: 'CXFile' }): number {
  return addon.clang_isFileMultipleIncludeGuarded(tu, file);
}

/**
 * Retrieve a file handle within the given translation unit.

\param tu the translation unit

\param file_name the name of the file.

\returns the file handle for the named file in the translation unit \p tu,
or a NULL file handle if the file was not a part of this translation unit.
 * @param tu - CXTranslationUnit
 * @param file_name - const char *
 * @returns CXFile
 */
export function clang_getFile(tu: { _type: 'CXTranslationUnit' }, file_name: string): { _type: 'CXFile' } {
  return addon.clang_getFile(tu, file_name);
}

/**
 * Retrieve the buffer associated with the given file.

\param tu the translation unit

\param file the file for which to retrieve the buffer.

\param size [out] if non-NULL, will be set to the size of the buffer.

\returns a pointer to the buffer in memory that holds the contents of
\p file, or a NULL pointer when the file is not loaded.
 * @param tu - CXTranslationUnit
 * @param file - CXFile
 * @param size - int *
 * @returns const char *
 */
export function clang_getFileContents(tu: { _type: 'CXTranslationUnit' }, file: { _type: 'CXFile' }, size: { _ptr: unknown }): string {
  return addon.clang_getFileContents(tu, file, size);
}

/**
 * Retrieves the source location associated with a given file/line/column
in a particular translation unit.
 * @param tu - CXTranslationUnit
 * @param file - CXFile
 * @param line - unsigned int
 * @param column - unsigned int
 * @returns CXSourceLocation
 */
export function clang_getLocation(tu: { _type: 'CXTranslationUnit' }, file: { _type: 'CXFile' }, line: number, column: number): { _type: 'CXSourceLocation' } {
  return addon.clang_getLocation(tu, file, line, column);
}

/**
 * Retrieves the source location associated with a given character offset
in a particular translation unit.
 * @param tu - CXTranslationUnit
 * @param file - CXFile
 * @param offset - unsigned int
 * @returns CXSourceLocation
 */
export function clang_getLocationForOffset(tu: { _type: 'CXTranslationUnit' }, file: { _type: 'CXFile' }, offset: number): { _type: 'CXSourceLocation' } {
  return addon.clang_getLocationForOffset(tu, file, offset);
}

/**
 * Retrieve all ranges that were skipped by the preprocessor.

The preprocessor will skip lines when they are surrounded by an
if/ifdef/ifndef directive whose condition does not evaluate to true.
 * @param tu - CXTranslationUnit
 * @param file - CXFile
 * @returns CXSourceRangeList *
 */
export function clang_getSkippedRanges(tu: { _type: 'CXTranslationUnit' }, file: { _type: 'CXFile' }): { _ptr: unknown } {
  return addon.clang_getSkippedRanges(tu, file);
}

/**
 * Retrieve all ranges from all files that were skipped by the
preprocessor.

The preprocessor will skip lines when they are surrounded by an
if/ifdef/ifndef directive whose condition does not evaluate to true.
 * @param tu - CXTranslationUnit
 * @returns CXSourceRangeList *
 */
export function clang_getAllSkippedRanges(tu: { _type: 'CXTranslationUnit' }): { _ptr: unknown } {
  return addon.clang_getAllSkippedRanges(tu);
}

/**
 * Determine the number of diagnostics produced for the given
translation unit.
 * @param Unit - CXTranslationUnit
 * @returns unsigned int
 */
export function clang_getNumDiagnostics(Unit: { _type: 'CXTranslationUnit' }): number {
  return addon.clang_getNumDiagnostics(Unit);
}

/**
 * Retrieve a diagnostic associated with the given translation unit.

\param Unit the translation unit to query.
\param Index the zero-based diagnostic number to retrieve.

\returns the requested diagnostic. This diagnostic must be freed
via a call to \c clang_disposeDiagnostic().
 * @param Unit - CXTranslationUnit
 * @param Index - unsigned int
 * @returns CXDiagnostic
 */
export function clang_getDiagnostic(Unit: { _type: 'CXTranslationUnit' }, Index: number): { _type: 'CXDiagnostic' } {
  return addon.clang_getDiagnostic(Unit, Index);
}

/**
 * Retrieve the complete set of diagnostics associated with a
translation unit.

\param Unit the translation unit to query.
 * @param Unit - CXTranslationUnit
 * @returns CXDiagnosticSet
 */
export function clang_getDiagnosticSetFromTU(Unit: { _type: 'CXTranslationUnit' }): { _type: 'CXDiagnosticSet' } {
  return addon.clang_getDiagnosticSetFromTU(Unit);
}

/**
 * Get the original translation unit source file name.
 * @param CTUnit - CXTranslationUnit
 * @returns CXString
 */
export function clang_getTranslationUnitSpelling(CTUnit: { _type: 'CXTranslationUnit' }): { _type: 'CXString' } {
  return addon.clang_getTranslationUnitSpelling(CTUnit);
}

/**
 * Return the CXTranslationUnit for a given source file and the provided
command line arguments one would pass to the compiler.

Note: The 'source_filename' argument is optional.  If the caller provides a
NULL pointer, the name of the source file is expected to reside in the
specified command line arguments.

Note: When encountered in 'clang_command_line_args', the following options
are ignored:

'-c'
'-emit-ast'
'-fsyntax-only'
'-o \<output file>'  (both '-o' and '\<output file>' are ignored)

\param CIdx The index object with which the translation unit will be
associated.

\param source_filename The name of the source file to load, or NULL if the
source file is included in \p clang_command_line_args.

\param num_clang_command_line_args The number of command-line arguments in
\p clang_command_line_args.

\param clang_command_line_args The command-line arguments that would be
passed to the \c clang executable if it were being invoked out-of-process.
These command-line options will be parsed and will affect how the translation
unit is parsed. Note that the following options are ignored: '-c',
'-emit-ast', '-fsyntax-only' (which is the default), and '-o \<output file>'.

\param num_unsaved_files the number of unsaved file entries in \p
unsaved_files.

\param unsaved_files the files that have not yet been saved to disk
but may be required for code completion, including the contents of
those files.  The contents and name of these files (as specified by
CXUnsavedFile) are copied when necessary, so the client only needs to
guarantee their validity until the call to this function returns.
 * @param CIdx - CXIndex
 * @param source_filename - const char *
 * @param num_clang_command_line_args - int
 * @param clang_command_line_args - const char *const *
 * @param num_unsaved_files - unsigned int
 * @param unsaved_files - struct CXUnsavedFile *
 * @returns CXTranslationUnit
 */
export function clang_createTranslationUnitFromSourceFile(CIdx: { _type: 'CXIndex' }, source_filename: string, num_clang_command_line_args: number, clang_command_line_args: { _ptr: unknown }, num_unsaved_files: number, unsaved_files: { _ptr: unknown }): { _type: 'CXTranslationUnit' } {
  return addon.clang_createTranslationUnitFromSourceFile(CIdx, source_filename, num_clang_command_line_args, clang_command_line_args, num_unsaved_files, unsaved_files);
}

/**
 * Same as \c clang_createTranslationUnit2, but returns
the \c CXTranslationUnit instead of an error code.  In case of an error this
routine returns a \c NULL \c CXTranslationUnit, without further detailed
error codes.
 * @param CIdx - CXIndex
 * @param ast_filename - const char *
 * @returns CXTranslationUnit
 */
export function clang_createTranslationUnit(CIdx: { _type: 'CXIndex' }, ast_filename: string): { _type: 'CXTranslationUnit' } {
  return addon.clang_createTranslationUnit(CIdx, ast_filename);
}

/**
 * Create a translation unit from an AST file (\c -emit-ast).

\param[out] out_TU A non-NULL pointer to store the created
\c CXTranslationUnit.

\returns Zero on success, otherwise returns an error code.
 * @param CIdx - CXIndex
 * @param ast_filename - const char *
 * @param out_TU - CXTranslationUnit *
 * @returns enum CXErrorCode
 */
export function clang_createTranslationUnit2(CIdx: { _type: 'CXIndex' }, ast_filename: string, out_TU: { _ptr: unknown }): number {
  return addon.clang_createTranslationUnit2(CIdx, ast_filename, out_TU);
}

/**
 * Returns the set of flags that is suitable for parsing a translation
unit that is being edited.

The set of flags returned provide options for \c clang_parseTranslationUnit()
to indicate that the translation unit is likely to be reparsed many times,
either explicitly (via \c clang_reparseTranslationUnit()) or implicitly
(e.g., by code completion (\c clang_codeCompletionAt())). The returned flag
set contains an unspecified set of optimizations (e.g., the precompiled
preamble) geared toward improving the performance of these routines. The
set of optimizations enabled may change from one version to the next.
 * @returns unsigned int
 */
export function clang_defaultEditingTranslationUnitOptions(): number {
  return addon.clang_defaultEditingTranslationUnitOptions();
}

/**
 * Same as \c clang_parseTranslationUnit2, but returns
the \c CXTranslationUnit instead of an error code.  In case of an error this
routine returns a \c NULL \c CXTranslationUnit, without further detailed
error codes.
 * @param CIdx - CXIndex
 * @param source_filename - const char *
 * @param command_line_args - const char *const *
 * @param num_command_line_args - int
 * @param unsaved_files - struct CXUnsavedFile *
 * @param num_unsaved_files - unsigned int
 * @param options - unsigned int
 * @returns CXTranslationUnit
 */
export function clang_parseTranslationUnit(CIdx: { _type: 'CXIndex' }, source_filename: string, command_line_args: { _ptr: unknown }, num_command_line_args: number, unsaved_files: { _ptr: unknown }, num_unsaved_files: number, options: number): { _type: 'CXTranslationUnit' } {
  return addon.clang_parseTranslationUnit(CIdx, source_filename, command_line_args, num_command_line_args, unsaved_files, num_unsaved_files, options);
}

/**
 * Parse the given source file and the translation unit corresponding
to that file.

This routine is the main entry point for the Clang C API, providing the
ability to parse a source file into a translation unit that can then be
queried by other functions in the API. This routine accepts a set of
command-line arguments so that the compilation can be configured in the same
way that the compiler is configured on the command line.

\param CIdx The index object with which the translation unit will be
associated.

\param source_filename The name of the source file to load, or NULL if the
source file is included in \c command_line_args.

\param command_line_args The command-line arguments that would be
passed to the \c clang executable if it were being invoked out-of-process.
These command-line options will be parsed and will affect how the translation
unit is parsed. Note that the following options are ignored: '-c',
'-emit-ast', '-fsyntax-only' (which is the default), and '-o \<output file>'.

\param num_command_line_args The number of command-line arguments in
\c command_line_args.

\param unsaved_files the files that have not yet been saved to disk
but may be required for parsing, including the contents of
those files.  The contents and name of these files (as specified by
CXUnsavedFile) are copied when necessary, so the client only needs to
guarantee their validity until the call to this function returns.

\param num_unsaved_files the number of unsaved file entries in \p
unsaved_files.

\param options A bitmask of options that affects how the translation unit
is managed but not its compilation. This should be a bitwise OR of the
CXTranslationUnit_XXX flags.

\param[out] out_TU A non-NULL pointer to store the created
\c CXTranslationUnit, describing the parsed code and containing any
diagnostics produced by the compiler.

\returns Zero on success, otherwise returns an error code.
 * @param CIdx - CXIndex
 * @param source_filename - const char *
 * @param command_line_args - const char *const *
 * @param num_command_line_args - int
 * @param unsaved_files - struct CXUnsavedFile *
 * @param num_unsaved_files - unsigned int
 * @param options - unsigned int
 * @param out_TU - CXTranslationUnit *
 * @returns enum CXErrorCode
 */
export function clang_parseTranslationUnit2(CIdx: { _type: 'CXIndex' }, source_filename: string, command_line_args: { _ptr: unknown }, num_command_line_args: number, unsaved_files: { _ptr: unknown }, num_unsaved_files: number, options: number, out_TU: { _ptr: unknown }): number {
  return addon.clang_parseTranslationUnit2(CIdx, source_filename, command_line_args, num_command_line_args, unsaved_files, num_unsaved_files, options, out_TU);
}

/**
 * Same as clang_parseTranslationUnit2 but requires a full command line
for \c command_line_args including argv[0]. This is useful if the standard
library paths are relative to the binary.
 * @param CIdx - CXIndex
 * @param source_filename - const char *
 * @param command_line_args - const char *const *
 * @param num_command_line_args - int
 * @param unsaved_files - struct CXUnsavedFile *
 * @param num_unsaved_files - unsigned int
 * @param options - unsigned int
 * @param out_TU - CXTranslationUnit *
 * @returns enum CXErrorCode
 */
export function clang_parseTranslationUnit2FullArgv(CIdx: { _type: 'CXIndex' }, source_filename: string, command_line_args: { _ptr: unknown }, num_command_line_args: number, unsaved_files: { _ptr: unknown }, num_unsaved_files: number, options: number, out_TU: { _ptr: unknown }): number {
  return addon.clang_parseTranslationUnit2FullArgv(CIdx, source_filename, command_line_args, num_command_line_args, unsaved_files, num_unsaved_files, options, out_TU);
}

/**
 * Returns the set of flags that is suitable for saving a translation
unit.

The set of flags returned provide options for
\c clang_saveTranslationUnit() by default. The returned flag
set contains an unspecified set of options that save translation units with
the most commonly-requested data.
 * @param TU - CXTranslationUnit
 * @returns unsigned int
 */
export function clang_defaultSaveOptions(TU: { _type: 'CXTranslationUnit' }): number {
  return addon.clang_defaultSaveOptions(TU);
}

/**
 * Saves a translation unit into a serialized representation of
that translation unit on disk.

Any translation unit that was parsed without error can be saved
into a file. The translation unit can then be deserialized into a
new \c CXTranslationUnit with \c clang_createTranslationUnit() or,
if it is an incomplete translation unit that corresponds to a
header, used as a precompiled header when parsing other translation
units.

\param TU The translation unit to save.

\param FileName The file to which the translation unit will be saved.

\param options A bitmask of options that affects how the translation unit
is saved. This should be a bitwise OR of the
CXSaveTranslationUnit_XXX flags.

\returns A value that will match one of the enumerators of the CXSaveError
enumeration. Zero (CXSaveError_None) indicates that the translation unit was
saved successfully, while a non-zero value indicates that a problem occurred.
 * @param TU - CXTranslationUnit
 * @param FileName - const char *
 * @param options - unsigned int
 * @returns int
 */
export function clang_saveTranslationUnit(TU: { _type: 'CXTranslationUnit' }, FileName: string, options: number): number {
  return addon.clang_saveTranslationUnit(TU, FileName, options);
}

/**
 * Suspend a translation unit in order to free memory associated with it.

A suspended translation unit uses significantly less memory but on the other
side does not support any other calls than \c clang_reparseTranslationUnit
to resume it or \c clang_disposeTranslationUnit to dispose it completely.
 * @param arg - CXTranslationUnit
 * @returns unsigned int
 */
export function clang_suspendTranslationUnit(arg0: { _type: 'CXTranslationUnit' }): number {
  return addon.clang_suspendTranslationUnit(arg0);
}

/**
 * Destroy the specified CXTranslationUnit object.
 * @param arg - CXTranslationUnit
 */
export function clang_disposeTranslationUnit(arg0: { _type: 'CXTranslationUnit' }): void {
  addon.clang_disposeTranslationUnit(arg0);
}

/**
 * Returns the set of flags that is suitable for reparsing a translation
unit.

The set of flags returned provide options for
\c clang_reparseTranslationUnit() by default. The returned flag
set contains an unspecified set of optimizations geared toward common uses
of reparsing. The set of optimizations enabled may change from one version
to the next.
 * @param TU - CXTranslationUnit
 * @returns unsigned int
 */
export function clang_defaultReparseOptions(TU: { _type: 'CXTranslationUnit' }): number {
  return addon.clang_defaultReparseOptions(TU);
}

/**
 * Reparse the source files that produced this translation unit.

This routine can be used to re-parse the source files that originally
created the given translation unit, for example because those source files
have changed (either on disk or as passed via \p unsaved_files). The
source code will be reparsed with the same command-line options as it
was originally parsed.

Reparsing a translation unit invalidates all cursors and source locations
that refer into that translation unit. This makes reparsing a translation
unit semantically equivalent to destroying the translation unit and then
creating a new translation unit with the same command-line arguments.
However, it may be more efficient to reparse a translation
unit using this routine.

\param TU The translation unit whose contents will be re-parsed. The
translation unit must originally have been built with
\c clang_createTranslationUnitFromSourceFile().

\param num_unsaved_files The number of unsaved file entries in \p
unsaved_files.

\param unsaved_files The files that have not yet been saved to disk
but may be required for parsing, including the contents of
those files.  The contents and name of these files (as specified by
CXUnsavedFile) are copied when necessary, so the client only needs to
guarantee their validity until the call to this function returns.

\param options A bitset of options composed of the flags in CXReparse_Flags.
The function \c clang_defaultReparseOptions() produces a default set of
options recommended for most uses, based on the translation unit.

\returns 0 if the sources could be reparsed.  A non-zero error code will be
returned if reparsing was impossible, such that the translation unit is
invalid. In such cases, the only valid call for \c TU is
\c clang_disposeTranslationUnit(TU).  The error codes returned by this
routine are described by the \c CXErrorCode enum.
 * @param TU - CXTranslationUnit
 * @param num_unsaved_files - unsigned int
 * @param unsaved_files - struct CXUnsavedFile *
 * @param options - unsigned int
 * @returns int
 */
export function clang_reparseTranslationUnit(TU: { _type: 'CXTranslationUnit' }, num_unsaved_files: number, unsaved_files: { _ptr: unknown }, options: number): number {
  return addon.clang_reparseTranslationUnit(TU, num_unsaved_files, unsaved_files, options);
}

/**
 * Returns the human-readable null-terminated C string that represents
the name of the memory category.  This string should never be freed.
 * @param kind - enum CXTUResourceUsageKind
 * @returns const char *
 */
export function clang_getTUResourceUsageName(kind: number): string {
  return addon.clang_getTUResourceUsageName(kind);
}

/**
 * Return the memory usage of a translation unit.  This object
should be released with clang_disposeCXTUResourceUsage().
 * @param TU - CXTranslationUnit
 * @returns CXTUResourceUsage
 */
export function clang_getCXTUResourceUsage(TU: { _type: 'CXTranslationUnit' }): { _type: 'CXTUResourceUsage' } {
  return addon.clang_getCXTUResourceUsage(TU);
}

export function clang_disposeCXTUResourceUsage(usage: { _type: 'CXTUResourceUsage' }): void {
  addon.clang_disposeCXTUResourceUsage(usage);
}

/**
 * Get target information for this translation unit.

The CXTargetInfo object cannot outlive the CXTranslationUnit object.
 * @param CTUnit - CXTranslationUnit
 * @returns CXTargetInfo
 */
export function clang_getTranslationUnitTargetInfo(CTUnit: { _type: 'CXTranslationUnit' }): { _type: 'CXTargetInfo' } {
  return addon.clang_getTranslationUnitTargetInfo(CTUnit);
}

/**
 * Destroy the CXTargetInfo object.
 * @param Info - CXTargetInfo
 */
export function clang_TargetInfo_dispose(Info: { _type: 'CXTargetInfo' }): void {
  addon.clang_TargetInfo_dispose(Info);
}

/**
 * Get the normalized target triple as a string.

Returns the empty string in case of any error.
 * @param Info - CXTargetInfo
 * @returns CXString
 */
export function clang_TargetInfo_getTriple(Info: { _type: 'CXTargetInfo' }): { _type: 'CXString' } {
  return addon.clang_TargetInfo_getTriple(Info);
}

/**
 * Get the pointer width of the target in bits.

Returns -1 in case of error.
 * @param Info - CXTargetInfo
 * @returns int
 */
export function clang_TargetInfo_getPointerWidth(Info: { _type: 'CXTargetInfo' }): number {
  return addon.clang_TargetInfo_getPointerWidth(Info);
}

/**
 * Retrieve the NULL cursor, which represents no entity.
 * @returns CXCursor
 */
export function clang_getNullCursor(): { _type: 'CXCursor' } {
  return addon.clang_getNullCursor();
}

/**
 * Retrieve the cursor that represents the given translation unit.

The translation unit cursor can be used to start traversing the
various declarations within the given translation unit.
 * @param arg - CXTranslationUnit
 * @returns CXCursor
 */
export function clang_getTranslationUnitCursor(arg0: { _type: 'CXTranslationUnit' }): { _type: 'CXCursor' } {
  return addon.clang_getTranslationUnitCursor(arg0);
}

/**
 * Determine whether two cursors are equivalent.
 * @param arg - CXCursor
 * @param arg - CXCursor
 * @returns unsigned int
 */
export function clang_equalCursors(arg0: { _type: 'CXCursor' }, arg1: { _type: 'CXCursor' }): number {
  return addon.clang_equalCursors(arg0, arg1);
}

/**
 * Returns non-zero if \p cursor is null.
 * @param cursor - CXCursor
 * @returns int
 */
export function clang_Cursor_isNull(cursor: { _type: 'CXCursor' }): number {
  return addon.clang_Cursor_isNull(cursor);
}

/**
 * Compute a hash value for the given cursor.
 * @param arg - CXCursor
 * @returns unsigned int
 */
export function clang_hashCursor(arg0: { _type: 'CXCursor' }): number {
  return addon.clang_hashCursor(arg0);
}

/**
 * Retrieve the kind of the given cursor.
 * @param arg - CXCursor
 * @returns enum CXCursorKind
 */
export function clang_getCursorKind(arg0: { _type: 'CXCursor' }): number {
  return addon.clang_getCursorKind(arg0);
}

/**
 * Determine whether the given cursor kind represents a declaration.
 * @param arg - enum CXCursorKind
 * @returns unsigned int
 */
export function clang_isDeclaration(arg0: number): number {
  return addon.clang_isDeclaration(arg0);
}

/**
 * Determine whether the given declaration is invalid.

A declaration is invalid if it could not be parsed successfully.

\returns non-zero if the cursor represents a declaration and it is
invalid, otherwise NULL.
 * @param arg - CXCursor
 * @returns unsigned int
 */
export function clang_isInvalidDeclaration(arg0: { _type: 'CXCursor' }): number {
  return addon.clang_isInvalidDeclaration(arg0);
}

/**
 * Determine whether the given cursor kind represents a simple
reference.

Note that other kinds of cursors (such as expressions) can also refer to
other cursors. Use clang_getCursorReferenced() to determine whether a
particular cursor refers to another entity.
 * @param arg - enum CXCursorKind
 * @returns unsigned int
 */
export function clang_isReference(arg0: number): number {
  return addon.clang_isReference(arg0);
}

/**
 * Determine whether the given cursor kind represents an expression.
 * @param arg - enum CXCursorKind
 * @returns unsigned int
 */
export function clang_isExpression(arg0: number): number {
  return addon.clang_isExpression(arg0);
}

/**
 * Determine whether the given cursor kind represents a statement.
 * @param arg - enum CXCursorKind
 * @returns unsigned int
 */
export function clang_isStatement(arg0: number): number {
  return addon.clang_isStatement(arg0);
}

/**
 * Determine whether the given cursor kind represents an attribute.
 * @param arg - enum CXCursorKind
 * @returns unsigned int
 */
export function clang_isAttribute(arg0: number): number {
  return addon.clang_isAttribute(arg0);
}

/**
 * Determine whether the given cursor has any attributes.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_Cursor_hasAttrs(C: { _type: 'CXCursor' }): number {
  return addon.clang_Cursor_hasAttrs(C);
}

/**
 * Determine whether the given cursor kind represents an invalid
cursor.
 * @param arg - enum CXCursorKind
 * @returns unsigned int
 */
export function clang_isInvalid(arg0: number): number {
  return addon.clang_isInvalid(arg0);
}

/**
 * Determine whether the given cursor kind represents a translation
unit.
 * @param arg - enum CXCursorKind
 * @returns unsigned int
 */
export function clang_isTranslationUnit(arg0: number): number {
  return addon.clang_isTranslationUnit(arg0);
}

/**
 * *
Determine whether the given cursor represents a preprocessing
element, such as a preprocessor directive or macro instantiation.
 * @param arg - enum CXCursorKind
 * @returns unsigned int
 */
export function clang_isPreprocessing(arg0: number): number {
  return addon.clang_isPreprocessing(arg0);
}

/**
 * *
Determine whether the given cursor represents a currently
unexposed piece of the AST (e.g., CXCursor_UnexposedStmt).
 * @param arg - enum CXCursorKind
 * @returns unsigned int
 */
export function clang_isUnexposed(arg0: number): number {
  return addon.clang_isUnexposed(arg0);
}

/**
 * Determine the linkage of the entity referred to by a given cursor.
 * @param cursor - CXCursor
 * @returns enum CXLinkageKind
 */
export function clang_getCursorLinkage(cursor: { _type: 'CXCursor' }): number {
  return addon.clang_getCursorLinkage(cursor);
}

/**
 * Describe the visibility of the entity referred to by a cursor.

This returns the default visibility if not explicitly specified by
a visibility attribute. The default visibility may be changed by
commandline arguments.

\param cursor The cursor to query.

\returns The visibility of the cursor.
 * @param cursor - CXCursor
 * @returns enum CXVisibilityKind
 */
export function clang_getCursorVisibility(cursor: { _type: 'CXCursor' }): number {
  return addon.clang_getCursorVisibility(cursor);
}

/**
 * Determine the availability of the entity that this cursor refers to,
taking the current target platform into account.

\param cursor The cursor to query.

\returns The availability of the cursor.
 * @param cursor - CXCursor
 * @returns enum CXAvailabilityKind
 */
export function clang_getCursorAvailability(cursor: { _type: 'CXCursor' }): number {
  return addon.clang_getCursorAvailability(cursor);
}

/**
 * Determine the availability of the entity that this cursor refers to
on any platforms for which availability information is known.

\param cursor The cursor to query.

\param always_deprecated If non-NULL, will be set to indicate whether the
entity is deprecated on all platforms.

\param deprecated_message If non-NULL, will be set to the message text
provided along with the unconditional deprecation of this entity. The client
is responsible for deallocating this string.

\param always_unavailable If non-NULL, will be set to indicate whether the
entity is unavailable on all platforms.

\param unavailable_message If non-NULL, will be set to the message text
provided along with the unconditional unavailability of this entity. The
client is responsible for deallocating this string.

\param availability If non-NULL, an array of CXPlatformAvailability instances
that will be populated with platform availability information, up to either
the number of platforms for which availability information is available (as
returned by this function) or \c availability_size, whichever is smaller.

\param availability_size The number of elements available in the
\c availability array.

\returns The number of platforms (N) for which availability information is
available (which is unrelated to \c availability_size).

Note that the client is responsible for calling
\c clang_disposeCXPlatformAvailability to free each of the
platform-availability structures returned. There are
\c min(N, availability_size) such structures.
 * @param cursor - CXCursor
 * @param always_deprecated - int *
 * @param deprecated_message - CXString *
 * @param always_unavailable - int *
 * @param unavailable_message - CXString *
 * @param availability - CXPlatformAvailability *
 * @param availability_size - int
 * @returns int
 */
export function clang_getCursorPlatformAvailability(cursor: { _type: 'CXCursor' }, always_deprecated: { _ptr: unknown }, deprecated_message: { _ptr: unknown }, always_unavailable: { _ptr: unknown }, unavailable_message: { _ptr: unknown }, availability: { _ptr: unknown }, availability_size: number): number {
  return addon.clang_getCursorPlatformAvailability(cursor, always_deprecated, deprecated_message, always_unavailable, unavailable_message, availability, availability_size);
}

/**
 * Free the memory associated with a \c CXPlatformAvailability structure.
 * @param availability - CXPlatformAvailability *
 */
export function clang_disposeCXPlatformAvailability(availability: { _ptr: unknown }): void {
  addon.clang_disposeCXPlatformAvailability(availability);
}

/**
 * If cursor refers to a variable declaration and it has initializer returns
cursor referring to the initializer otherwise return null cursor.
 * @param cursor - CXCursor
 * @returns CXCursor
 */
export function clang_Cursor_getVarDeclInitializer(cursor: { _type: 'CXCursor' }): { _type: 'CXCursor' } {
  return addon.clang_Cursor_getVarDeclInitializer(cursor);
}

/**
 * If cursor refers to a variable declaration that has global storage returns 1.
If cursor refers to a variable declaration that doesn't have global storage
returns 0. Otherwise returns -1.
 * @param cursor - CXCursor
 * @returns int
 */
export function clang_Cursor_hasVarDeclGlobalStorage(cursor: { _type: 'CXCursor' }): number {
  return addon.clang_Cursor_hasVarDeclGlobalStorage(cursor);
}

/**
 * If cursor refers to a variable declaration that has external storage
returns 1. If cursor refers to a variable declaration that doesn't have
external storage returns 0. Otherwise returns -1.
 * @param cursor - CXCursor
 * @returns int
 */
export function clang_Cursor_hasVarDeclExternalStorage(cursor: { _type: 'CXCursor' }): number {
  return addon.clang_Cursor_hasVarDeclExternalStorage(cursor);
}

/**
 * Determine the "language" of the entity referred to by a given cursor.
 * @param cursor - CXCursor
 * @returns enum CXLanguageKind
 */
export function clang_getCursorLanguage(cursor: { _type: 'CXCursor' }): number {
  return addon.clang_getCursorLanguage(cursor);
}

/**
 * Determine the "thread-local storage (TLS) kind" of the declaration
referred to by a cursor.
 * @param cursor - CXCursor
 * @returns enum CXTLSKind
 */
export function clang_getCursorTLSKind(cursor: { _type: 'CXCursor' }): number {
  return addon.clang_getCursorTLSKind(cursor);
}

/**
 * Returns the translation unit that a cursor originated from.
 * @param arg - CXCursor
 * @returns CXTranslationUnit
 */
export function clang_Cursor_getTranslationUnit(arg0: { _type: 'CXCursor' }): { _type: 'CXTranslationUnit' } {
  return addon.clang_Cursor_getTranslationUnit(arg0);
}

/**
 * Creates an empty CXCursorSet.
 * @returns CXCursorSet
 */
export function clang_createCXCursorSet(): { _type: 'CXCursorSet' } {
  return addon.clang_createCXCursorSet();
}

/**
 * Disposes a CXCursorSet and releases its associated memory.
 * @param cset - CXCursorSet
 */
export function clang_disposeCXCursorSet(cset: { _type: 'CXCursorSet' }): void {
  addon.clang_disposeCXCursorSet(cset);
}

/**
 * Queries a CXCursorSet to see if it contains a specific CXCursor.

\returns non-zero if the set contains the specified cursor.
 * @param cset - CXCursorSet
 * @param cursor - CXCursor
 * @returns unsigned int
 */
export function clang_CXCursorSet_contains(cset: { _type: 'CXCursorSet' }, cursor: { _type: 'CXCursor' }): number {
  return addon.clang_CXCursorSet_contains(cset, cursor);
}

/**
 * Inserts a CXCursor into a CXCursorSet.

\returns zero if the CXCursor was already in the set, and non-zero otherwise.
 * @param cset - CXCursorSet
 * @param cursor - CXCursor
 * @returns unsigned int
 */
export function clang_CXCursorSet_insert(cset: { _type: 'CXCursorSet' }, cursor: { _type: 'CXCursor' }): number {
  return addon.clang_CXCursorSet_insert(cset, cursor);
}

/**
 * Determine the semantic parent of the given cursor.

The semantic parent of a cursor is the cursor that semantically contains
the given \p cursor. For many declarations, the lexical and semantic parents
are equivalent (the lexical parent is returned by
\c clang_getCursorLexicalParent()). They diverge when declarations or
definitions are provided out-of-line. For example:

\code
class C {
void f();
};

void C::f() { }
\endcode

In the out-of-line definition of \c C::f, the semantic parent is
the class \c C, of which this function is a member. The lexical parent is
the place where the declaration actually occurs in the source code; in this
case, the definition occurs in the translation unit. In general, the
lexical parent for a given entity can change without affecting the semantics
of the program, and the lexical parent of different declarations of the
same entity may be different. Changing the semantic parent of a declaration,
on the other hand, can have a major impact on semantics, and redeclarations
of a particular entity should all have the same semantic context.

In the example above, both declarations of \c C::f have \c C as their
semantic context, while the lexical context of the first \c C::f is \c C
and the lexical context of the second \c C::f is the translation unit.

For global declarations, the semantic parent is the translation unit.
 * @param cursor - CXCursor
 * @returns CXCursor
 */
export function clang_getCursorSemanticParent(cursor: { _type: 'CXCursor' }): { _type: 'CXCursor' } {
  return addon.clang_getCursorSemanticParent(cursor);
}

/**
 * Determine the lexical parent of the given cursor.

The lexical parent of a cursor is the cursor in which the given \p cursor
was actually written. For many declarations, the lexical and semantic parents
are equivalent (the semantic parent is returned by
\c clang_getCursorSemanticParent()). They diverge when declarations or
definitions are provided out-of-line. For example:

\code
class C {
void f();
};

void C::f() { }
\endcode

In the out-of-line definition of \c C::f, the semantic parent is
the class \c C, of which this function is a member. The lexical parent is
the place where the declaration actually occurs in the source code; in this
case, the definition occurs in the translation unit. In general, the
lexical parent for a given entity can change without affecting the semantics
of the program, and the lexical parent of different declarations of the
same entity may be different. Changing the semantic parent of a declaration,
on the other hand, can have a major impact on semantics, and redeclarations
of a particular entity should all have the same semantic context.

In the example above, both declarations of \c C::f have \c C as their
semantic context, while the lexical context of the first \c C::f is \c C
and the lexical context of the second \c C::f is the translation unit.

For declarations written in the global scope, the lexical parent is
the translation unit.
 * @param cursor - CXCursor
 * @returns CXCursor
 */
export function clang_getCursorLexicalParent(cursor: { _type: 'CXCursor' }): { _type: 'CXCursor' } {
  return addon.clang_getCursorLexicalParent(cursor);
}

/**
 * Determine the set of methods that are overridden by the given
method.

In both Objective-C and C++, a method (aka virtual member function,
in C++) can override a virtual method in a base class. For
Objective-C, a method is said to override any method in the class's
base class, its protocols, or its categories' protocols, that has the same
selector and is of the same kind (class or instance).
If no such method exists, the search continues to the class's superclass,
its protocols, and its categories, and so on. A method from an Objective-C
implementation is considered to override the same methods as its
corresponding method in the interface.

For C++, a virtual member function overrides any virtual member
function with the same signature that occurs in its base
classes. With multiple inheritance, a virtual member function can
override several virtual member functions coming from different
base classes.

In all cases, this function determines the immediate overridden
method, rather than all of the overridden methods. For example, if
a method is originally declared in a class A, then overridden in B
(which in inherits from A) and also in C (which inherited from B),
then the only overridden method returned from this function when
invoked on C's method will be B's method. The client may then
invoke this function again, given the previously-found overridden
methods, to map out the complete method-override set.

\param cursor A cursor representing an Objective-C or C++
method. This routine will compute the set of methods that this
method overrides.

\param overridden A pointer whose pointee will be replaced with a
pointer to an array of cursors, representing the set of overridden
methods. If there are no overridden methods, the pointee will be
set to NULL. The pointee must be freed via a call to
\c clang_disposeOverriddenCursors().

\param num_overridden A pointer to the number of overridden
functions, will be set to the number of overridden functions in the
array pointed to by \p overridden.
 * @param cursor - CXCursor
 * @param overridden - CXCursor **
 * @param num_overridden - unsigned int *
 */
export function clang_getOverriddenCursors(cursor: { _type: 'CXCursor' }, overridden: { _ptr: unknown }, num_overridden: { _ptr: unknown }): void {
  addon.clang_getOverriddenCursors(cursor, overridden, num_overridden);
}

/**
 * Free the set of overridden cursors returned by \c
clang_getOverriddenCursors().
 * @param overridden - CXCursor *
 */
export function clang_disposeOverriddenCursors(overridden: { _ptr: unknown }): void {
  addon.clang_disposeOverriddenCursors(overridden);
}

/**
 * Retrieve the file that is included by the given inclusion directive
cursor.
 * @param cursor - CXCursor
 * @returns CXFile
 */
export function clang_getIncludedFile(cursor: { _type: 'CXCursor' }): { _type: 'CXFile' } {
  return addon.clang_getIncludedFile(cursor);
}

/**
 * Map a source location to the cursor that describes the entity at that
location in the source code.

clang_getCursor() maps an arbitrary source location within a translation
unit down to the most specific cursor that describes the entity at that
location. For example, given an expression \c x + y, invoking
clang_getCursor() with a source location pointing to "x" will return the
cursor for "x"; similarly for "y". If the cursor points anywhere between
"x" or "y" (e.g., on the + or the whitespace around it), clang_getCursor()
will return a cursor referring to the "+" expression.

\returns a cursor representing the entity at the given source location, or
a NULL cursor if no such entity can be found.
 * @param arg - CXTranslationUnit
 * @param arg - CXSourceLocation
 * @returns CXCursor
 */
export function clang_getCursor(arg0: { _type: 'CXTranslationUnit' }, arg1: { _type: 'CXSourceLocation' }): { _type: 'CXCursor' } {
  return addon.clang_getCursor(arg0, arg1);
}

/**
 * Retrieve the physical location of the source constructor referenced
by the given cursor.

The location of a declaration is typically the location of the name of that
declaration, where the name of that declaration would occur if it is
unnamed, or some keyword that introduces that particular declaration.
The location of a reference is where that reference occurs within the
source code.
 * @param arg - CXCursor
 * @returns CXSourceLocation
 */
export function clang_getCursorLocation(arg0: { _type: 'CXCursor' }): { _type: 'CXSourceLocation' } {
  return addon.clang_getCursorLocation(arg0);
}

/**
 * Retrieve the physical extent of the source construct referenced by
the given cursor.

The extent of a cursor starts with the file/line/column pointing at the
first character within the source construct that the cursor refers to and
ends with the last character within that source construct. For a
declaration, the extent covers the declaration itself. For a reference,
the extent covers the location of the reference (e.g., where the referenced
entity was actually used).
 * @param arg - CXCursor
 * @returns CXSourceRange
 */
export function clang_getCursorExtent(arg0: { _type: 'CXCursor' }): { _type: 'CXSourceRange' } {
  return addon.clang_getCursorExtent(arg0);
}

/**
 * Retrieve the type of a CXCursor (if any).
 * @param C - CXCursor
 * @returns CXType
 */
export function clang_getCursorType(C: { _type: 'CXCursor' }): { _type: 'CXType' } {
  return addon.clang_getCursorType(C);
}

/**
 * Pretty-print the underlying type using the rules of the
language of the translation unit from which it came.

If the type is invalid, an empty string is returned.
 * @param CT - CXType
 * @returns CXString
 */
export function clang_getTypeSpelling(CT: { _type: 'CXType' }): { _type: 'CXString' } {
  return addon.clang_getTypeSpelling(CT);
}

/**
 * Retrieve the underlying type of a typedef declaration.

If the cursor does not reference a typedef declaration, an invalid type is
returned.
 * @param C - CXCursor
 * @returns CXType
 */
export function clang_getTypedefDeclUnderlyingType(C: { _type: 'CXCursor' }): { _type: 'CXType' } {
  return addon.clang_getTypedefDeclUnderlyingType(C);
}

/**
 * Retrieve the integer type of an enum declaration.

If the cursor does not reference an enum declaration, an invalid type is
returned.
 * @param C - CXCursor
 * @returns CXType
 */
export function clang_getEnumDeclIntegerType(C: { _type: 'CXCursor' }): { _type: 'CXType' } {
  return addon.clang_getEnumDeclIntegerType(C);
}

/**
 * Retrieve the integer value of an enum constant declaration as a signed
long long.

If the cursor does not reference an enum constant declaration, LLONG_MIN is
returned. Since this is also potentially a valid constant value, the kind of
the cursor must be verified before calling this function.
 * @param C - CXCursor
 * @returns long long
 */
export function clang_getEnumConstantDeclValue(C: { _type: 'CXCursor' }): bigint {
  return addon.clang_getEnumConstantDeclValue(C);
}

/**
 * Retrieve the integer value of an enum constant declaration as an unsigned
long long.

If the cursor does not reference an enum constant declaration, ULLONG_MAX is
returned. Since this is also potentially a valid constant value, the kind of
the cursor must be verified before calling this function.
 * @param C - CXCursor
 * @returns unsigned long long
 */
export function clang_getEnumConstantDeclUnsignedValue(C: { _type: 'CXCursor' }): bigint {
  return addon.clang_getEnumConstantDeclUnsignedValue(C);
}

/**
 * Returns non-zero if the cursor specifies a Record member that is a bit-field.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_Cursor_isBitField(C: { _type: 'CXCursor' }): number {
  return addon.clang_Cursor_isBitField(C);
}

/**
 * Retrieve the bit width of a bit-field declaration as an integer.

If the cursor does not reference a bit-field, or if the bit-field's width
expression cannot be evaluated, -1 is returned.

For example:
\code
if (clang_Cursor_isBitField(Cursor)) {
int Width = clang_getFieldDeclBitWidth(Cursor);
if (Width != -1) {
// The bit-field width is not value-dependent.
}
}
\endcode
 * @param C - CXCursor
 * @returns int
 */
export function clang_getFieldDeclBitWidth(C: { _type: 'CXCursor' }): number {
  return addon.clang_getFieldDeclBitWidth(C);
}

/**
 * Retrieve the number of non-variadic arguments associated with a given
cursor.

The number of arguments can be determined for calls as well as for
declarations of functions or methods. For other cursors -1 is returned.
 * @param C - CXCursor
 * @returns int
 */
export function clang_Cursor_getNumArguments(C: { _type: 'CXCursor' }): number {
  return addon.clang_Cursor_getNumArguments(C);
}

/**
 * Retrieve the argument cursor of a function or method.

The argument cursor can be determined for calls as well as for declarations
of functions or methods. For other cursors and for invalid indices, an
invalid cursor is returned.
 * @param C - CXCursor
 * @param i - unsigned int
 * @returns CXCursor
 */
export function clang_Cursor_getArgument(C: { _type: 'CXCursor' }, i: number): { _type: 'CXCursor' } {
  return addon.clang_Cursor_getArgument(C, i);
}

/**
 * Returns the number of template args of a function, struct, or class decl
representing a template specialization.

If the argument cursor cannot be converted into a template function
declaration, -1 is returned.

For example, for the following declaration and specialization:
template <typename T, int kInt, bool kBool>
void foo() { ... }

template <>
void foo<float, -7, true>();

The value 3 would be returned from this call.
 * @param C - CXCursor
 * @returns int
 */
export function clang_Cursor_getNumTemplateArguments(C: { _type: 'CXCursor' }): number {
  return addon.clang_Cursor_getNumTemplateArguments(C);
}

/**
 * Retrieve the kind of the I'th template argument of the CXCursor C.

If the argument CXCursor does not represent a FunctionDecl, StructDecl, or
ClassTemplatePartialSpecialization, an invalid template argument kind is
returned.

For example, for the following declaration and specialization:
template <typename T, int kInt, bool kBool>
void foo() { ... }

template <>
void foo<float, -7, true>();

For I = 0, 1, and 2, Type, Integral, and Integral will be returned,
respectively.
 * @param C - CXCursor
 * @param I - unsigned int
 * @returns enum CXTemplateArgumentKind
 */
export function clang_Cursor_getTemplateArgumentKind(C: { _type: 'CXCursor' }, I: number): number {
  return addon.clang_Cursor_getTemplateArgumentKind(C, I);
}

/**
 * Retrieve a CXType representing the type of a TemplateArgument of a
function decl representing a template specialization.

If the argument CXCursor does not represent a FunctionDecl, StructDecl,
ClassDecl or ClassTemplatePartialSpecialization whose I'th template argument
has a kind of CXTemplateArgKind_Integral, an invalid type is returned.

For example, for the following declaration and specialization:
template <typename T, int kInt, bool kBool>
void foo() { ... }

template <>
void foo<float, -7, true>();

If called with I = 0, "float", will be returned.
Invalid types will be returned for I == 1 or 2.
 * @param C - CXCursor
 * @param I - unsigned int
 * @returns CXType
 */
export function clang_Cursor_getTemplateArgumentType(C: { _type: 'CXCursor' }, I: number): { _type: 'CXType' } {
  return addon.clang_Cursor_getTemplateArgumentType(C, I);
}

/**
 * Retrieve the value of an Integral TemplateArgument (of a function
decl representing a template specialization) as a signed long long.

It is undefined to call this function on a CXCursor that does not represent a
FunctionDecl, StructDecl, ClassDecl or ClassTemplatePartialSpecialization
whose I'th template argument is not an integral value.

For example, for the following declaration and specialization:
template <typename T, int kInt, bool kBool>
void foo() { ... }

template <>
void foo<float, -7, true>();

If called with I = 1 or 2, -7 or true will be returned, respectively.
For I == 0, this function's behavior is undefined.
 * @param C - CXCursor
 * @param I - unsigned int
 * @returns long long
 */
export function clang_Cursor_getTemplateArgumentValue(C: { _type: 'CXCursor' }, I: number): bigint {
  return addon.clang_Cursor_getTemplateArgumentValue(C, I);
}

/**
 * Retrieve the value of an Integral TemplateArgument (of a function
decl representing a template specialization) as an unsigned long long.

It is undefined to call this function on a CXCursor that does not represent a
FunctionDecl, StructDecl, ClassDecl or ClassTemplatePartialSpecialization or
whose I'th template argument is not an integral value.

For example, for the following declaration and specialization:
template <typename T, int kInt, bool kBool>
void foo() { ... }

template <>
void foo<float, 2147483649, true>();

If called with I = 1 or 2, 2147483649 or true will be returned, respectively.
For I == 0, this function's behavior is undefined.
 * @param C - CXCursor
 * @param I - unsigned int
 * @returns unsigned long long
 */
export function clang_Cursor_getTemplateArgumentUnsignedValue(C: { _type: 'CXCursor' }, I: number): bigint {
  return addon.clang_Cursor_getTemplateArgumentUnsignedValue(C, I);
}

/**
 * Determine whether two CXTypes represent the same type.

\returns non-zero if the CXTypes represent the same type and
zero otherwise.
 * @param A - CXType
 * @param B - CXType
 * @returns unsigned int
 */
export function clang_equalTypes(A: { _type: 'CXType' }, B: { _type: 'CXType' }): number {
  return addon.clang_equalTypes(A, B);
}

/**
 * Return the canonical type for a CXType.

Clang's type system explicitly models typedefs and all the ways
a specific type can be represented.  The canonical type is the underlying
type with all the "sugar" removed.  For example, if 'T' is a typedef
for 'int', the canonical type for 'T' would be 'int'.
 * @param T - CXType
 * @returns CXType
 */
export function clang_getCanonicalType(T: { _type: 'CXType' }): { _type: 'CXType' } {
  return addon.clang_getCanonicalType(T);
}

/**
 * Determine whether a CXType has the "const" qualifier set,
without looking through typedefs that may have added "const" at a
different level.
 * @param T - CXType
 * @returns unsigned int
 */
export function clang_isConstQualifiedType(T: { _type: 'CXType' }): number {
  return addon.clang_isConstQualifiedType(T);
}

/**
 * Determine whether a  CXCursor that is a macro, is
function like.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_Cursor_isMacroFunctionLike(C: { _type: 'CXCursor' }): number {
  return addon.clang_Cursor_isMacroFunctionLike(C);
}

/**
 * Determine whether a  CXCursor that is a macro, is a
builtin one.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_Cursor_isMacroBuiltin(C: { _type: 'CXCursor' }): number {
  return addon.clang_Cursor_isMacroBuiltin(C);
}

/**
 * Determine whether a  CXCursor that is a function declaration, is an
inline declaration.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_Cursor_isFunctionInlined(C: { _type: 'CXCursor' }): number {
  return addon.clang_Cursor_isFunctionInlined(C);
}

/**
 * Determine whether a CXType has the "volatile" qualifier set,
without looking through typedefs that may have added "volatile" at
a different level.
 * @param T - CXType
 * @returns unsigned int
 */
export function clang_isVolatileQualifiedType(T: { _type: 'CXType' }): number {
  return addon.clang_isVolatileQualifiedType(T);
}

/**
 * Determine whether a CXType has the "restrict" qualifier set,
without looking through typedefs that may have added "restrict" at a
different level.
 * @param T - CXType
 * @returns unsigned int
 */
export function clang_isRestrictQualifiedType(T: { _type: 'CXType' }): number {
  return addon.clang_isRestrictQualifiedType(T);
}

/**
 * Returns the address space of the given type.
 * @param T - CXType
 * @returns unsigned int
 */
export function clang_getAddressSpace(T: { _type: 'CXType' }): number {
  return addon.clang_getAddressSpace(T);
}

/**
 * Returns the typedef name of the given type.
 * @param CT - CXType
 * @returns CXString
 */
export function clang_getTypedefName(CT: { _type: 'CXType' }): { _type: 'CXString' } {
  return addon.clang_getTypedefName(CT);
}

/**
 * For pointer types, returns the type of the pointee.
 * @param T - CXType
 * @returns CXType
 */
export function clang_getPointeeType(T: { _type: 'CXType' }): { _type: 'CXType' } {
  return addon.clang_getPointeeType(T);
}

/**
 * Retrieve the unqualified variant of the given type, removing as
little sugar as possible.

For example, given the following series of typedefs:

\code
typedef int Integer;
typedef const Integer CInteger;
typedef CInteger DifferenceType;
\endcode

Executing \c clang_getUnqualifiedType() on a \c CXType that
represents \c DifferenceType, will desugar to a type representing
\c Integer, that has no qualifiers.

And, executing \c clang_getUnqualifiedType() on the type of the
first argument of the following function declaration:

\code
void foo(const int);
\endcode

Will return a type representing \c int, removing the \c const
qualifier.

Sugar over array types is not desugared.

A type can be checked for qualifiers with \c
clang_isConstQualifiedType(), \c clang_isVolatileQualifiedType()
and \c clang_isRestrictQualifiedType().

A type that resulted from a call to \c clang_getUnqualifiedType
will return \c false for all of the above calls.
 * @param CT - CXType
 * @returns CXType
 */
export function clang_getUnqualifiedType(CT: { _type: 'CXType' }): { _type: 'CXType' } {
  return addon.clang_getUnqualifiedType(CT);
}

/**
 * For reference types (e.g., "const int&"), returns the type that the
reference refers to (e.g "const int").

Otherwise, returns the type itself.

A type that has kind \c CXType_LValueReference or
\c CXType_RValueReference is a reference type.
 * @param CT - CXType
 * @returns CXType
 */
export function clang_getNonReferenceType(CT: { _type: 'CXType' }): { _type: 'CXType' } {
  return addon.clang_getNonReferenceType(CT);
}

/**
 * Return the cursor for the declaration of the given type.
 * @param T - CXType
 * @returns CXCursor
 */
export function clang_getTypeDeclaration(T: { _type: 'CXType' }): { _type: 'CXCursor' } {
  return addon.clang_getTypeDeclaration(T);
}

/**
 * Returns the Objective-C type encoding for the specified declaration.
 * @param C - CXCursor
 * @returns CXString
 */
export function clang_getDeclObjCTypeEncoding(C: { _type: 'CXCursor' }): { _type: 'CXString' } {
  return addon.clang_getDeclObjCTypeEncoding(C);
}

/**
 * Returns the Objective-C type encoding for the specified CXType.
 * @param type - CXType
 * @returns CXString
 */
export function clang_Type_getObjCEncoding(type: { _type: 'CXType' }): { _type: 'CXString' } {
  return addon.clang_Type_getObjCEncoding(type);
}

/**
 * Retrieve the spelling of a given CXTypeKind.
 * @param K - enum CXTypeKind
 * @returns CXString
 */
export function clang_getTypeKindSpelling(K: number): { _type: 'CXString' } {
  return addon.clang_getTypeKindSpelling(K);
}

/**
 * Retrieve the calling convention associated with a function type.

If a non-function type is passed in, CXCallingConv_Invalid is returned.
 * @param T - CXType
 * @returns enum CXCallingConv
 */
export function clang_getFunctionTypeCallingConv(T: { _type: 'CXType' }): number {
  return addon.clang_getFunctionTypeCallingConv(T);
}

/**
 * Retrieve the return type associated with a function type.

If a non-function type is passed in, an invalid type is returned.
 * @param T - CXType
 * @returns CXType
 */
export function clang_getResultType(T: { _type: 'CXType' }): { _type: 'CXType' } {
  return addon.clang_getResultType(T);
}

/**
 * Retrieve the exception specification type associated with a function type.
This is a value of type CXCursor_ExceptionSpecificationKind.

If a non-function type is passed in, an error code of -1 is returned.
 * @param T - CXType
 * @returns int
 */
export function clang_getExceptionSpecificationType(T: { _type: 'CXType' }): number {
  return addon.clang_getExceptionSpecificationType(T);
}

/**
 * Retrieve the number of non-variadic parameters associated with a
function type.

If a non-function type is passed in, -1 is returned.
 * @param T - CXType
 * @returns int
 */
export function clang_getNumArgTypes(T: { _type: 'CXType' }): number {
  return addon.clang_getNumArgTypes(T);
}

/**
 * Retrieve the type of a parameter of a function type.

If a non-function type is passed in or the function does not have enough
parameters, an invalid type is returned.
 * @param T - CXType
 * @param i - unsigned int
 * @returns CXType
 */
export function clang_getArgType(T: { _type: 'CXType' }, i: number): { _type: 'CXType' } {
  return addon.clang_getArgType(T, i);
}

/**
 * Retrieves the base type of the ObjCObjectType.

If the type is not an ObjC object, an invalid type is returned.
 * @param T - CXType
 * @returns CXType
 */
export function clang_Type_getObjCObjectBaseType(T: { _type: 'CXType' }): { _type: 'CXType' } {
  return addon.clang_Type_getObjCObjectBaseType(T);
}

/**
 * Retrieve the number of protocol references associated with an ObjC object/id.

If the type is not an ObjC object, 0 is returned.
 * @param T - CXType
 * @returns unsigned int
 */
export function clang_Type_getNumObjCProtocolRefs(T: { _type: 'CXType' }): number {
  return addon.clang_Type_getNumObjCProtocolRefs(T);
}

/**
 * Retrieve the decl for a protocol reference for an ObjC object/id.

If the type is not an ObjC object or there are not enough protocol
references, an invalid cursor is returned.
 * @param T - CXType
 * @param i - unsigned int
 * @returns CXCursor
 */
export function clang_Type_getObjCProtocolDecl(T: { _type: 'CXType' }, i: number): { _type: 'CXCursor' } {
  return addon.clang_Type_getObjCProtocolDecl(T, i);
}

/**
 * Retrieve the number of type arguments associated with an ObjC object.

If the type is not an ObjC object, 0 is returned.
 * @param T - CXType
 * @returns unsigned int
 */
export function clang_Type_getNumObjCTypeArgs(T: { _type: 'CXType' }): number {
  return addon.clang_Type_getNumObjCTypeArgs(T);
}

/**
 * Retrieve a type argument associated with an ObjC object.

If the type is not an ObjC or the index is not valid,
an invalid type is returned.
 * @param T - CXType
 * @param i - unsigned int
 * @returns CXType
 */
export function clang_Type_getObjCTypeArg(T: { _type: 'CXType' }, i: number): { _type: 'CXType' } {
  return addon.clang_Type_getObjCTypeArg(T, i);
}

/**
 * Return 1 if the CXType is a variadic function type, and 0 otherwise.
 * @param T - CXType
 * @returns unsigned int
 */
export function clang_isFunctionTypeVariadic(T: { _type: 'CXType' }): number {
  return addon.clang_isFunctionTypeVariadic(T);
}

/**
 * Retrieve the return type associated with a given cursor.

This only returns a valid type if the cursor refers to a function or method.
 * @param C - CXCursor
 * @returns CXType
 */
export function clang_getCursorResultType(C: { _type: 'CXCursor' }): { _type: 'CXType' } {
  return addon.clang_getCursorResultType(C);
}

/**
 * Retrieve the exception specification type associated with a given cursor.
This is a value of type CXCursor_ExceptionSpecificationKind.

This only returns a valid result if the cursor refers to a function or
method.
 * @param C - CXCursor
 * @returns int
 */
export function clang_getCursorExceptionSpecificationType(C: { _type: 'CXCursor' }): number {
  return addon.clang_getCursorExceptionSpecificationType(C);
}

/**
 * Return 1 if the CXType is a POD (plain old data) type, and 0
otherwise.
 * @param T - CXType
 * @returns unsigned int
 */
export function clang_isPODType(T: { _type: 'CXType' }): number {
  return addon.clang_isPODType(T);
}

/**
 * Return the element type of an array, complex, or vector type.

If a type is passed in that is not an array, complex, or vector type,
an invalid type is returned.
 * @param T - CXType
 * @returns CXType
 */
export function clang_getElementType(T: { _type: 'CXType' }): { _type: 'CXType' } {
  return addon.clang_getElementType(T);
}

/**
 * Return the number of elements of an array or vector type.

If a type is passed in that is not an array or vector type,
-1 is returned.
 * @param T - CXType
 * @returns long long
 */
export function clang_getNumElements(T: { _type: 'CXType' }): bigint {
  return addon.clang_getNumElements(T);
}

/**
 * Return the element type of an array type.

If a non-array type is passed in, an invalid type is returned.
 * @param T - CXType
 * @returns CXType
 */
export function clang_getArrayElementType(T: { _type: 'CXType' }): { _type: 'CXType' } {
  return addon.clang_getArrayElementType(T);
}

/**
 * Return the array size of a constant array.

If a non-array type is passed in, -1 is returned.
 * @param T - CXType
 * @returns long long
 */
export function clang_getArraySize(T: { _type: 'CXType' }): bigint {
  return addon.clang_getArraySize(T);
}

/**
 * Retrieve the type named by the qualified-id.

If a non-elaborated type is passed in, an invalid type is returned.
 * @param T - CXType
 * @returns CXType
 */
export function clang_Type_getNamedType(T: { _type: 'CXType' }): { _type: 'CXType' } {
  return addon.clang_Type_getNamedType(T);
}

/**
 * Determine if a typedef is 'transparent' tag.

A typedef is considered 'transparent' if it shares a name and spelling
location with its underlying tag type, as is the case with the NS_ENUM macro.

\returns non-zero if transparent and zero otherwise.
 * @param T - CXType
 * @returns unsigned int
 */
export function clang_Type_isTransparentTagTypedef(T: { _type: 'CXType' }): number {
  return addon.clang_Type_isTransparentTagTypedef(T);
}

/**
 * Retrieve the nullability kind of a pointer type.
 * @param T - CXType
 * @returns enum CXTypeNullabilityKind
 */
export function clang_Type_getNullability(T: { _type: 'CXType' }): number {
  return addon.clang_Type_getNullability(T);
}

/**
 * Return the alignment of a type in bytes as per C++[expr.alignof]
standard.

If the type declaration is invalid, CXTypeLayoutError_Invalid is returned.
If the type declaration is an incomplete type, CXTypeLayoutError_Incomplete
is returned.
If the type declaration is a dependent type, CXTypeLayoutError_Dependent is
returned.
If the type declaration is not a constant size type,
CXTypeLayoutError_NotConstantSize is returned.
 * @param T - CXType
 * @returns long long
 */
export function clang_Type_getAlignOf(T: { _type: 'CXType' }): bigint {
  return addon.clang_Type_getAlignOf(T);
}

/**
 * Return the class type of an member pointer type.

If a non-member-pointer type is passed in, an invalid type is returned.
 * @param T - CXType
 * @returns CXType
 */
export function clang_Type_getClassType(T: { _type: 'CXType' }): { _type: 'CXType' } {
  return addon.clang_Type_getClassType(T);
}

/**
 * Return the size of a type in bytes as per C++[expr.sizeof] standard.

If the type declaration is invalid, CXTypeLayoutError_Invalid is returned.
If the type declaration is an incomplete type, CXTypeLayoutError_Incomplete
is returned.
If the type declaration is a dependent type, CXTypeLayoutError_Dependent is
returned.
 * @param T - CXType
 * @returns long long
 */
export function clang_Type_getSizeOf(T: { _type: 'CXType' }): bigint {
  return addon.clang_Type_getSizeOf(T);
}

/**
 * Return the offset of a field named S in a record of type T in bits
as it would be returned by __offsetof__ as per C++11[18.2p4]

If the cursor is not a record field declaration, CXTypeLayoutError_Invalid
is returned.
If the field's type declaration is an incomplete type,
CXTypeLayoutError_Incomplete is returned.
If the field's type declaration is a dependent type,
CXTypeLayoutError_Dependent is returned.
If the field's name S is not found,
CXTypeLayoutError_InvalidFieldName is returned.
 * @param T - CXType
 * @param S - const char *
 * @returns long long
 */
export function clang_Type_getOffsetOf(T: { _type: 'CXType' }, S: string): bigint {
  return addon.clang_Type_getOffsetOf(T, S);
}

/**
 * Return the type that was modified by this attributed type.

If the type is not an attributed type, an invalid type is returned.
 * @param T - CXType
 * @returns CXType
 */
export function clang_Type_getModifiedType(T: { _type: 'CXType' }): { _type: 'CXType' } {
  return addon.clang_Type_getModifiedType(T);
}

/**
 * Gets the type contained by this atomic type.

If a non-atomic type is passed in, an invalid type is returned.
 * @param CT - CXType
 * @returns CXType
 */
export function clang_Type_getValueType(CT: { _type: 'CXType' }): { _type: 'CXType' } {
  return addon.clang_Type_getValueType(CT);
}

/**
 * Return the offset of the field represented by the Cursor.

If the cursor is not a field declaration, -1 is returned.
If the cursor semantic parent is not a record field declaration,
CXTypeLayoutError_Invalid is returned.
If the field's type declaration is an incomplete type,
CXTypeLayoutError_Incomplete is returned.
If the field's type declaration is a dependent type,
CXTypeLayoutError_Dependent is returned.
If the field's name S is not found,
CXTypeLayoutError_InvalidFieldName is returned.
 * @param C - CXCursor
 * @returns long long
 */
export function clang_Cursor_getOffsetOfField(C: { _type: 'CXCursor' }): bigint {
  return addon.clang_Cursor_getOffsetOfField(C);
}

/**
 * Determine whether the given cursor represents an anonymous
tag or namespace
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_Cursor_isAnonymous(C: { _type: 'CXCursor' }): number {
  return addon.clang_Cursor_isAnonymous(C);
}

/**
 * Determine whether the given cursor represents an anonymous record
declaration.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_Cursor_isAnonymousRecordDecl(C: { _type: 'CXCursor' }): number {
  return addon.clang_Cursor_isAnonymousRecordDecl(C);
}

/**
 * Determine whether the given cursor represents an inline namespace
declaration.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_Cursor_isInlineNamespace(C: { _type: 'CXCursor' }): number {
  return addon.clang_Cursor_isInlineNamespace(C);
}

/**
 * Returns the number of template arguments for given template
specialization, or -1 if type \c T is not a template specialization.
 * @param T - CXType
 * @returns int
 */
export function clang_Type_getNumTemplateArguments(T: { _type: 'CXType' }): number {
  return addon.clang_Type_getNumTemplateArguments(T);
}

/**
 * Returns the type template argument of a template class specialization
at given index.

This function only returns template type arguments and does not handle
template template arguments or variadic packs.
 * @param T - CXType
 * @param i - unsigned int
 * @returns CXType
 */
export function clang_Type_getTemplateArgumentAsType(T: { _type: 'CXType' }, i: number): { _type: 'CXType' } {
  return addon.clang_Type_getTemplateArgumentAsType(T, i);
}

/**
 * Retrieve the ref-qualifier kind of a function or method.

The ref-qualifier is returned for C++ functions or methods. For other types
or non-C++ declarations, CXRefQualifier_None is returned.
 * @param T - CXType
 * @returns enum CXRefQualifierKind
 */
export function clang_Type_getCXXRefQualifier(T: { _type: 'CXType' }): number {
  return addon.clang_Type_getCXXRefQualifier(T);
}

/**
 * Returns 1 if the base class specified by the cursor with kind
CX_CXXBaseSpecifier is virtual.
 * @param arg - CXCursor
 * @returns unsigned int
 */
export function clang_isVirtualBase(arg0: { _type: 'CXCursor' }): number {
  return addon.clang_isVirtualBase(arg0);
}

/**
 * Returns the offset in bits of a CX_CXXBaseSpecifier relative to the parent
class.

Returns a small negative number if the offset cannot be computed. See
CXTypeLayoutError for error codes.
 * @param Parent - CXCursor
 * @param Base - CXCursor
 * @returns long long
 */
export function clang_getOffsetOfBase(Parent: { _type: 'CXCursor' }, Base: { _type: 'CXCursor' }): bigint {
  return addon.clang_getOffsetOfBase(Parent, Base);
}

/**
 * Returns the access control level for the referenced object.

If the cursor refers to a C++ declaration, its access control level within
its parent scope is returned. Otherwise, if the cursor refers to a base
specifier or access specifier, the specifier itself is returned.
 * @param arg - CXCursor
 * @returns enum CX_CXXAccessSpecifier
 */
export function clang_getCXXAccessSpecifier(arg0: { _type: 'CXCursor' }): number {
  return addon.clang_getCXXAccessSpecifier(arg0);
}

/**
 * \brief Returns the operator code for the binary operator.
 * @param C - CXCursor
 * @returns enum CX_BinaryOperatorKind
 */
export function clang_Cursor_getBinaryOpcode(C: { _type: 'CXCursor' }): number {
  return addon.clang_Cursor_getBinaryOpcode(C);
}

/**
 * \brief Returns a string containing the spelling of the binary operator.
 * @param Op - enum CX_BinaryOperatorKind
 * @returns CXString
 */
export function clang_Cursor_getBinaryOpcodeStr(Op: number): { _type: 'CXString' } {
  return addon.clang_Cursor_getBinaryOpcodeStr(Op);
}

/**
 * Returns the storage class for a function or variable declaration.

If the passed in Cursor is not a function or variable declaration,
CX_SC_Invalid is returned else the storage class.
 * @param arg - CXCursor
 * @returns enum CX_StorageClass
 */
export function clang_Cursor_getStorageClass(arg0: { _type: 'CXCursor' }): number {
  return addon.clang_Cursor_getStorageClass(arg0);
}

/**
 * Determine the number of overloaded declarations referenced by a
\c CXCursor_OverloadedDeclRef cursor.

\param cursor The cursor whose overloaded declarations are being queried.

\returns The number of overloaded declarations referenced by \c cursor. If it
is not a \c CXCursor_OverloadedDeclRef cursor, returns 0.
 * @param cursor - CXCursor
 * @returns unsigned int
 */
export function clang_getNumOverloadedDecls(cursor: { _type: 'CXCursor' }): number {
  return addon.clang_getNumOverloadedDecls(cursor);
}

/**
 * Retrieve a cursor for one of the overloaded declarations referenced
by a \c CXCursor_OverloadedDeclRef cursor.

\param cursor The cursor whose overloaded declarations are being queried.

\param index The zero-based index into the set of overloaded declarations in
the cursor.

\returns A cursor representing the declaration referenced by the given
\c cursor at the specified \c index. If the cursor does not have an
associated set of overloaded declarations, or if the index is out of bounds,
returns \c clang_getNullCursor();
 * @param cursor - CXCursor
 * @param index - unsigned int
 * @returns CXCursor
 */
export function clang_getOverloadedDecl(cursor: { _type: 'CXCursor' }, index: number): { _type: 'CXCursor' } {
  return addon.clang_getOverloadedDecl(cursor, index);
}

/**
 * For cursors representing an iboutletcollection attribute,
this function returns the collection element type.
 * @param arg - CXCursor
 * @returns CXType
 */
export function clang_getIBOutletCollectionType(arg0: { _type: 'CXCursor' }): { _type: 'CXType' } {
  return addon.clang_getIBOutletCollectionType(arg0);
}

/**
 * Visit the children of a particular cursor.

This function visits all the direct children of the given cursor,
invoking the given \p visitor function with the cursors of each
visited child. The traversal may be recursive, if the visitor returns
\c CXChildVisit_Recurse. The traversal may also be ended prematurely, if
the visitor returns \c CXChildVisit_Break.

\param parent the cursor whose child may be visited. All kinds of
cursors can be visited, including invalid cursors (which, by
definition, have no children).

\param visitor the visitor function that will be invoked for each
child of \p parent.

\param client_data pointer data supplied by the client, which will
be passed to the visitor each time it is invoked.

\returns a non-zero value if the traversal was terminated
prematurely by the visitor returning \c CXChildVisit_Break.
 * @param parent - CXCursor
 * @param visitor - CXCursorVisitor
 * @param client_data - CXClientData
 * @returns unsigned int
 */
export function clang_visitChildren(parent: { _type: 'CXCursor' }, visitor: { _type: 'CXCursorVisitor' }, client_data: { _type: 'CXClientData' }): number {
  return addon.clang_visitChildren(parent, visitor, client_data);
}

/**
 * Visits the children of a cursor using the specified block.  Behaves
identically to clang_visitChildren() in all other respects.
 * @param parent - CXCursor
 * @param block - CXCursorVisitorBlock
 * @returns unsigned int
 */
export function clang_visitChildrenWithBlock(parent: { _type: 'CXCursor' }, block: { _type: 'CXCursorVisitorBlock' }): number {
  return addon.clang_visitChildrenWithBlock(parent, block);
}

/**
 * Retrieve a Unified Symbol Resolution (USR) for the entity referenced
by the given cursor.

A Unified Symbol Resolution (USR) is a string that identifies a particular
entity (function, class, variable, etc.) within a program. USRs can be
compared across translation units to determine, e.g., when references in
one translation refer to an entity defined in another translation unit.
 * @param arg - CXCursor
 * @returns CXString
 */
export function clang_getCursorUSR(arg0: { _type: 'CXCursor' }): { _type: 'CXString' } {
  return addon.clang_getCursorUSR(arg0);
}

/**
 * Construct a USR for a specified Objective-C class.
 * @param class_name - const char *
 * @returns CXString
 */
export function clang_constructUSR_ObjCClass(class_name: string): { _type: 'CXString' } {
  return addon.clang_constructUSR_ObjCClass(class_name);
}

/**
 * Construct a USR for a specified Objective-C category.
 * @param class_name - const char *
 * @param category_name - const char *
 * @returns CXString
 */
export function clang_constructUSR_ObjCCategory(class_name: string, category_name: string): { _type: 'CXString' } {
  return addon.clang_constructUSR_ObjCCategory(class_name, category_name);
}

/**
 * Construct a USR for a specified Objective-C protocol.
 * @param protocol_name - const char *
 * @returns CXString
 */
export function clang_constructUSR_ObjCProtocol(protocol_name: string): { _type: 'CXString' } {
  return addon.clang_constructUSR_ObjCProtocol(protocol_name);
}

/**
 * Construct a USR for a specified Objective-C instance variable and
the USR for its containing class.
 * @param name - const char *
 * @param classUSR - CXString
 * @returns CXString
 */
export function clang_constructUSR_ObjCIvar(name: string, classUSR: { _type: 'CXString' }): { _type: 'CXString' } {
  return addon.clang_constructUSR_ObjCIvar(name, classUSR);
}

/**
 * Construct a USR for a specified Objective-C method and
the USR for its containing class.
 * @param name - const char *
 * @param isInstanceMethod - unsigned int
 * @param classUSR - CXString
 * @returns CXString
 */
export function clang_constructUSR_ObjCMethod(name: string, isInstanceMethod: number, classUSR: { _type: 'CXString' }): { _type: 'CXString' } {
  return addon.clang_constructUSR_ObjCMethod(name, isInstanceMethod, classUSR);
}

/**
 * Construct a USR for a specified Objective-C property and the USR
for its containing class.
 * @param property - const char *
 * @param classUSR - CXString
 * @returns CXString
 */
export function clang_constructUSR_ObjCProperty(property: string, classUSR: { _type: 'CXString' }): { _type: 'CXString' } {
  return addon.clang_constructUSR_ObjCProperty(property, classUSR);
}

/**
 * Retrieve a name for the entity referenced by this cursor.
 * @param arg - CXCursor
 * @returns CXString
 */
export function clang_getCursorSpelling(arg0: { _type: 'CXCursor' }): { _type: 'CXString' } {
  return addon.clang_getCursorSpelling(arg0);
}

/**
 * Retrieve a range for a piece that forms the cursors spelling name.
Most of the times there is only one range for the complete spelling but for
Objective-C methods and Objective-C message expressions, there are multiple
pieces for each selector identifier.

\param pieceIndex the index of the spelling name piece. If this is greater
than the actual number of pieces, it will return a NULL (invalid) range.

\param options Reserved.
 * @param arg - CXCursor
 * @param pieceIndex - unsigned int
 * @param options - unsigned int
 * @returns CXSourceRange
 */
export function clang_Cursor_getSpellingNameRange(arg0: { _type: 'CXCursor' }, pieceIndex: number, options: number): { _type: 'CXSourceRange' } {
  return addon.clang_Cursor_getSpellingNameRange(arg0, pieceIndex, options);
}

/**
 * Get a property value for the given printing policy.
 * @param Policy - CXPrintingPolicy
 * @param Property - enum CXPrintingPolicyProperty
 * @returns unsigned int
 */
export function clang_PrintingPolicy_getProperty(Policy: { _type: 'CXPrintingPolicy' }, Property: number): number {
  return addon.clang_PrintingPolicy_getProperty(Policy, Property);
}

/**
 * Set a property value for the given printing policy.
 * @param Policy - CXPrintingPolicy
 * @param Property - enum CXPrintingPolicyProperty
 * @param Value - unsigned int
 */
export function clang_PrintingPolicy_setProperty(Policy: { _type: 'CXPrintingPolicy' }, Property: number, Value: number): void {
  addon.clang_PrintingPolicy_setProperty(Policy, Property, Value);
}

/**
 * Retrieve the default policy for the cursor.

The policy should be released after use with \c
clang_PrintingPolicy_dispose.
 * @param arg - CXCursor
 * @returns CXPrintingPolicy
 */
export function clang_getCursorPrintingPolicy(arg0: { _type: 'CXCursor' }): { _type: 'CXPrintingPolicy' } {
  return addon.clang_getCursorPrintingPolicy(arg0);
}

/**
 * Release a printing policy.
 * @param Policy - CXPrintingPolicy
 */
export function clang_PrintingPolicy_dispose(Policy: { _type: 'CXPrintingPolicy' }): void {
  addon.clang_PrintingPolicy_dispose(Policy);
}

/**
 * Pretty print declarations.

\param Cursor The cursor representing a declaration.

\param Policy The policy to control the entities being printed. If
NULL, a default policy is used.

\returns The pretty printed declaration or the empty string for
other cursors.
 * @param Cursor - CXCursor
 * @param Policy - CXPrintingPolicy
 * @returns CXString
 */
export function clang_getCursorPrettyPrinted(Cursor: { _type: 'CXCursor' }, Policy: { _type: 'CXPrintingPolicy' }): { _type: 'CXString' } {
  return addon.clang_getCursorPrettyPrinted(Cursor, Policy);
}

/**
 * Pretty-print the underlying type using a custom printing policy.

If the type is invalid, an empty string is returned.
 * @param CT - CXType
 * @param cxPolicy - CXPrintingPolicy
 * @returns CXString
 */
export function clang_getTypePrettyPrinted(CT: { _type: 'CXType' }, cxPolicy: { _type: 'CXPrintingPolicy' }): { _type: 'CXString' } {
  return addon.clang_getTypePrettyPrinted(CT, cxPolicy);
}

/**
 * Retrieve the display name for the entity referenced by this cursor.

The display name contains extra information that helps identify the cursor,
such as the parameters of a function or template or the arguments of a
class template specialization.
 * @param arg - CXCursor
 * @returns CXString
 */
export function clang_getCursorDisplayName(arg0: { _type: 'CXCursor' }): { _type: 'CXString' } {
  return addon.clang_getCursorDisplayName(arg0);
}

/**
 * For a cursor that is a reference, retrieve a cursor representing the
entity that it references.

Reference cursors refer to other entities in the AST. For example, an
Objective-C superclass reference cursor refers to an Objective-C class.
This function produces the cursor for the Objective-C class from the
cursor for the superclass reference. If the input cursor is a declaration or
definition, it returns that declaration or definition unchanged.
Otherwise, returns the NULL cursor.
 * @param arg - CXCursor
 * @returns CXCursor
 */
export function clang_getCursorReferenced(arg0: { _type: 'CXCursor' }): { _type: 'CXCursor' } {
  return addon.clang_getCursorReferenced(arg0);
}

/**
 * For a cursor that is either a reference to or a declaration
of some entity, retrieve a cursor that describes the definition of
that entity.

Some entities can be declared multiple times within a translation
unit, but only one of those declarations can also be a
definition. For example, given:

\code
int f(int, int);
int g(int x, int y) { return f(x, y); }
int f(int a, int b) { return a + b; }
int f(int, int);
\endcode

there are three declarations of the function "f", but only the
second one is a definition. The clang_getCursorDefinition()
function will take any cursor pointing to a declaration of "f"
(the first or fourth lines of the example) or a cursor referenced
that uses "f" (the call to "f' inside "g") and will return a
declaration cursor pointing to the definition (the second "f"
declaration).

If given a cursor for which there is no corresponding definition,
e.g., because there is no definition of that entity within this
translation unit, returns a NULL cursor.
 * @param arg - CXCursor
 * @returns CXCursor
 */
export function clang_getCursorDefinition(arg0: { _type: 'CXCursor' }): { _type: 'CXCursor' } {
  return addon.clang_getCursorDefinition(arg0);
}

/**
 * Determine whether the declaration pointed to by this cursor
is also a definition of that entity.
 * @param arg - CXCursor
 * @returns unsigned int
 */
export function clang_isCursorDefinition(arg0: { _type: 'CXCursor' }): number {
  return addon.clang_isCursorDefinition(arg0);
}

/**
 * Retrieve the canonical cursor corresponding to the given cursor.

In the C family of languages, many kinds of entities can be declared several
times within a single translation unit. For example, a structure type can
be forward-declared (possibly multiple times) and later defined:

\code
struct X;
struct X;
struct X {
int member;
};
\endcode

The declarations and the definition of \c X are represented by three
different cursors, all of which are declarations of the same underlying
entity. One of these cursor is considered the "canonical" cursor, which
is effectively the representative for the underlying entity. One can
determine if two cursors are declarations of the same underlying entity by
comparing their canonical cursors.

\returns The canonical cursor for the entity referred to by the given cursor.
 * @param arg - CXCursor
 * @returns CXCursor
 */
export function clang_getCanonicalCursor(arg0: { _type: 'CXCursor' }): { _type: 'CXCursor' } {
  return addon.clang_getCanonicalCursor(arg0);
}

/**
 * If the cursor points to a selector identifier in an Objective-C
method or message expression, this returns the selector index.

After getting a cursor with #clang_getCursor, this can be called to
determine if the location points to a selector identifier.

\returns The selector index if the cursor is an Objective-C method or message
expression and the cursor is pointing to a selector identifier, or -1
otherwise.
 * @param arg - CXCursor
 * @returns int
 */
export function clang_Cursor_getObjCSelectorIndex(arg0: { _type: 'CXCursor' }): number {
  return addon.clang_Cursor_getObjCSelectorIndex(arg0);
}

/**
 * Given a cursor pointing to a C++ method call or an Objective-C
message, returns non-zero if the method/message is "dynamic", meaning:

For a C++ method: the call is virtual.
For an Objective-C message: the receiver is an object instance, not 'super'
or a specific class.

If the method/message is "static" or the cursor does not point to a
method/message, it will return zero.
 * @param C - CXCursor
 * @returns int
 */
export function clang_Cursor_isDynamicCall(C: { _type: 'CXCursor' }): number {
  return addon.clang_Cursor_isDynamicCall(C);
}

/**
 * Given a cursor pointing to an Objective-C message or property
reference, or C++ method call, returns the CXType of the receiver.
 * @param C - CXCursor
 * @returns CXType
 */
export function clang_Cursor_getReceiverType(C: { _type: 'CXCursor' }): { _type: 'CXType' } {
  return addon.clang_Cursor_getReceiverType(C);
}

/**
 * Given a cursor that represents a property declaration, return the
associated property attributes. The bits are formed from
\c CXObjCPropertyAttrKind.

\param reserved Reserved for future use, pass 0.
 * @param C - CXCursor
 * @param reserved - unsigned int
 * @returns unsigned int
 */
export function clang_Cursor_getObjCPropertyAttributes(C: { _type: 'CXCursor' }, reserved: number): number {
  return addon.clang_Cursor_getObjCPropertyAttributes(C, reserved);
}

/**
 * Given a cursor that represents a property declaration, return the
name of the method that implements the getter.
 * @param C - CXCursor
 * @returns CXString
 */
export function clang_Cursor_getObjCPropertyGetterName(C: { _type: 'CXCursor' }): { _type: 'CXString' } {
  return addon.clang_Cursor_getObjCPropertyGetterName(C);
}

/**
 * Given a cursor that represents a property declaration, return the
name of the method that implements the setter, if any.
 * @param C - CXCursor
 * @returns CXString
 */
export function clang_Cursor_getObjCPropertySetterName(C: { _type: 'CXCursor' }): { _type: 'CXString' } {
  return addon.clang_Cursor_getObjCPropertySetterName(C);
}

/**
 * Given a cursor that represents an Objective-C method or parameter
declaration, return the associated Objective-C qualifiers for the return
type or the parameter respectively. The bits are formed from
CXObjCDeclQualifierKind.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_Cursor_getObjCDeclQualifiers(C: { _type: 'CXCursor' }): number {
  return addon.clang_Cursor_getObjCDeclQualifiers(C);
}

/**
 * Given a cursor that represents an Objective-C method or property
declaration, return non-zero if the declaration was affected by "\@optional".
Returns zero if the cursor is not such a declaration or it is "\@required".
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_Cursor_isObjCOptional(C: { _type: 'CXCursor' }): number {
  return addon.clang_Cursor_isObjCOptional(C);
}

/**
 * Returns non-zero if the given cursor is a variadic function or method.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_Cursor_isVariadic(C: { _type: 'CXCursor' }): number {
  return addon.clang_Cursor_isVariadic(C);
}

/**
 * Returns non-zero if the given cursor points to a symbol marked with
external_source_symbol attribute.

\param language If non-NULL, and the attribute is present, will be set to
the 'language' string from the attribute.

\param definedIn If non-NULL, and the attribute is present, will be set to
the 'definedIn' string from the attribute.

\param isGenerated If non-NULL, and the attribute is present, will be set to
non-zero if the 'generated_declaration' is set in the attribute.
 * @param C - CXCursor
 * @param language - CXString *
 * @param definedIn - CXString *
 * @param isGenerated - unsigned int *
 * @returns unsigned int
 */
export function clang_Cursor_isExternalSymbol(C: { _type: 'CXCursor' }, language: { _ptr: unknown }, definedIn: { _ptr: unknown }, isGenerated: { _ptr: unknown }): number {
  return addon.clang_Cursor_isExternalSymbol(C, language, definedIn, isGenerated);
}

/**
 * Given a cursor that represents a declaration, return the associated
comment's source range.  The range may include multiple consecutive comments
with whitespace in between.
 * @param C - CXCursor
 * @returns CXSourceRange
 */
export function clang_Cursor_getCommentRange(C: { _type: 'CXCursor' }): { _type: 'CXSourceRange' } {
  return addon.clang_Cursor_getCommentRange(C);
}

/**
 * Given a cursor that represents a declaration, return the associated
comment text, including comment markers.
 * @param C - CXCursor
 * @returns CXString
 */
export function clang_Cursor_getRawCommentText(C: { _type: 'CXCursor' }): { _type: 'CXString' } {
  return addon.clang_Cursor_getRawCommentText(C);
}

/**
 * Given a cursor that represents a documentable entity (e.g.,
declaration), return the associated \paragraph; otherwise return the
first paragraph.
 * @param C - CXCursor
 * @returns CXString
 */
export function clang_Cursor_getBriefCommentText(C: { _type: 'CXCursor' }): { _type: 'CXString' } {
  return addon.clang_Cursor_getBriefCommentText(C);
}

/**
 * Retrieve the CXString representing the mangled name of the cursor.
 * @param arg - CXCursor
 * @returns CXString
 */
export function clang_Cursor_getMangling(arg0: { _type: 'CXCursor' }): { _type: 'CXString' } {
  return addon.clang_Cursor_getMangling(arg0);
}

/**
 * Retrieve the CXStrings representing the mangled symbols of the C++
constructor or destructor at the cursor.
 * @param arg - CXCursor
 * @returns CXStringSet *
 */
export function clang_Cursor_getCXXManglings(arg0: { _type: 'CXCursor' }): { _ptr: unknown } {
  return addon.clang_Cursor_getCXXManglings(arg0);
}

/**
 * Retrieve the CXStrings representing the mangled symbols of the ObjC
class interface or implementation at the cursor.
 * @param arg - CXCursor
 * @returns CXStringSet *
 */
export function clang_Cursor_getObjCManglings(arg0: { _type: 'CXCursor' }): { _ptr: unknown } {
  return addon.clang_Cursor_getObjCManglings(arg0);
}

/**
 * Given a CXCursor_ModuleImportDecl cursor, return the associated module.
 * @param C - CXCursor
 * @returns CXModule
 */
export function clang_Cursor_getModule(C: { _type: 'CXCursor' }): { _type: 'CXModule' } {
  return addon.clang_Cursor_getModule(C);
}

/**
 * Given a CXFile header file, return the module that contains it, if one
exists.
 * @param arg - CXTranslationUnit
 * @param arg - CXFile
 * @returns CXModule
 */
export function clang_getModuleForFile(arg0: { _type: 'CXTranslationUnit' }, arg1: { _type: 'CXFile' }): { _type: 'CXModule' } {
  return addon.clang_getModuleForFile(arg0, arg1);
}

/**
 * \param Module a module object.

\returns the module file where the provided module object came from.
 * @param Module - CXModule
 * @returns CXFile
 */
export function clang_Module_getASTFile(Module: { _type: 'CXModule' }): { _type: 'CXFile' } {
  return addon.clang_Module_getASTFile(Module);
}

/**
 * \param Module a module object.

\returns the parent of a sub-module or NULL if the given module is top-level,
e.g. for 'std.vector' it will return the 'std' module.
 * @param Module - CXModule
 * @returns CXModule
 */
export function clang_Module_getParent(Module: { _type: 'CXModule' }): { _type: 'CXModule' } {
  return addon.clang_Module_getParent(Module);
}

/**
 * \param Module a module object.

\returns the name of the module, e.g. for the 'std.vector' sub-module it
will return "vector".
 * @param Module - CXModule
 * @returns CXString
 */
export function clang_Module_getName(Module: { _type: 'CXModule' }): { _type: 'CXString' } {
  return addon.clang_Module_getName(Module);
}

/**
 * \param Module a module object.

\returns the full name of the module, e.g. "std.vector".
 * @param Module - CXModule
 * @returns CXString
 */
export function clang_Module_getFullName(Module: { _type: 'CXModule' }): { _type: 'CXString' } {
  return addon.clang_Module_getFullName(Module);
}

/**
 * \param Module a module object.

\returns non-zero if the module is a system one.
 * @param Module - CXModule
 * @returns int
 */
export function clang_Module_isSystem(Module: { _type: 'CXModule' }): number {
  return addon.clang_Module_isSystem(Module);
}

/**
 * \param Module a module object.

\returns the number of top level headers associated with this module.
 * @param arg - CXTranslationUnit
 * @param Module - CXModule
 * @returns unsigned int
 */
export function clang_Module_getNumTopLevelHeaders(arg0: { _type: 'CXTranslationUnit' }, Module: { _type: 'CXModule' }): number {
  return addon.clang_Module_getNumTopLevelHeaders(arg0, Module);
}

/**
 * \param Module a module object.

\param Index top level header index (zero-based).

\returns the specified top level header associated with the module.
 * @param arg - CXTranslationUnit
 * @param Module - CXModule
 * @param Index - unsigned int
 * @returns CXFile
 */
export function clang_Module_getTopLevelHeader(arg0: { _type: 'CXTranslationUnit' }, Module: { _type: 'CXModule' }, Index: number): { _type: 'CXFile' } {
  return addon.clang_Module_getTopLevelHeader(arg0, Module, Index);
}

/**
 * Determine if a C++ constructor is a converting constructor.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_CXXConstructor_isConvertingConstructor(C: { _type: 'CXCursor' }): number {
  return addon.clang_CXXConstructor_isConvertingConstructor(C);
}

/**
 * Determine if a C++ constructor is a copy constructor.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_CXXConstructor_isCopyConstructor(C: { _type: 'CXCursor' }): number {
  return addon.clang_CXXConstructor_isCopyConstructor(C);
}

/**
 * Determine if a C++ constructor is the default constructor.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_CXXConstructor_isDefaultConstructor(C: { _type: 'CXCursor' }): number {
  return addon.clang_CXXConstructor_isDefaultConstructor(C);
}

/**
 * Determine if a C++ constructor is a move constructor.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_CXXConstructor_isMoveConstructor(C: { _type: 'CXCursor' }): number {
  return addon.clang_CXXConstructor_isMoveConstructor(C);
}

/**
 * Determine if a C++ field is declared 'mutable'.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_CXXField_isMutable(C: { _type: 'CXCursor' }): number {
  return addon.clang_CXXField_isMutable(C);
}

/**
 * Determine if a C++ method is declared '= default'.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_CXXMethod_isDefaulted(C: { _type: 'CXCursor' }): number {
  return addon.clang_CXXMethod_isDefaulted(C);
}

/**
 * Determine if a C++ method is declared '= delete'.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_CXXMethod_isDeleted(C: { _type: 'CXCursor' }): number {
  return addon.clang_CXXMethod_isDeleted(C);
}

/**
 * Determine if a C++ member function or member function template is
pure virtual.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_CXXMethod_isPureVirtual(C: { _type: 'CXCursor' }): number {
  return addon.clang_CXXMethod_isPureVirtual(C);
}

/**
 * Determine if a C++ member function or member function template is
declared 'static'.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_CXXMethod_isStatic(C: { _type: 'CXCursor' }): number {
  return addon.clang_CXXMethod_isStatic(C);
}

/**
 * Determine if a C++ member function or member function template is
explicitly declared 'virtual' or if it overrides a virtual method from
one of the base classes.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_CXXMethod_isVirtual(C: { _type: 'CXCursor' }): number {
  return addon.clang_CXXMethod_isVirtual(C);
}

/**
 * Determine if a C++ member function is a copy-assignment operator,
returning 1 if such is the case and 0 otherwise.

> A copy-assignment operator `X::operator=` is a non-static,
> non-template member function of _class_ `X` with exactly one
> parameter of type `X`, `X&`, `const X&`, `volatile X&` or `const
> volatile X&`.

That is, for example, the `operator=` in:

class Foo {
bool operator=(const volatile Foo&);
};

Is a copy-assignment operator, while the `operator=` in:

class Bar {
bool operator=(const int&);
};

Is not.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_CXXMethod_isCopyAssignmentOperator(C: { _type: 'CXCursor' }): number {
  return addon.clang_CXXMethod_isCopyAssignmentOperator(C);
}

/**
 * Determine if a C++ member function is a move-assignment operator,
returning 1 if such is the case and 0 otherwise.

> A move-assignment operator `X::operator=` is a non-static,
> non-template member function of _class_ `X` with exactly one
> parameter of type `X&&`, `const X&&`, `volatile X&&` or `const
> volatile X&&`.

That is, for example, the `operator=` in:

class Foo {
bool operator=(const volatile Foo&&);
};

Is a move-assignment operator, while the `operator=` in:

class Bar {
bool operator=(const int&&);
};

Is not.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_CXXMethod_isMoveAssignmentOperator(C: { _type: 'CXCursor' }): number {
  return addon.clang_CXXMethod_isMoveAssignmentOperator(C);
}

/**
 * Determines if a C++ constructor or conversion function was declared
explicit, returning 1 if such is the case and 0 otherwise.

Constructors or conversion functions are declared explicit through
the use of the explicit specifier.

For example, the following constructor and conversion function are
not explicit as they lack the explicit specifier:

class Foo {
Foo();
operator int();
};

While the following constructor and conversion function are
explicit as they are declared with the explicit specifier.

class Foo {
explicit Foo();
explicit operator int();
};

This function will return 0 when given a cursor pointing to one of
the former declarations and it will return 1 for a cursor pointing
to the latter declarations.

The explicit specifier allows the user to specify a
conditional compile-time expression whose value decides
whether the marked element is explicit or not.

For example:

constexpr bool foo(int i) { return i % 2 == 0; }

class Foo {
explicit(foo(1)) Foo();
explicit(foo(2)) operator int();
}

This function will return 0 for the constructor and 1 for
the conversion function.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_CXXMethod_isExplicit(C: { _type: 'CXCursor' }): number {
  return addon.clang_CXXMethod_isExplicit(C);
}

/**
 * Determine if a C++ record is abstract, i.e. whether a class or struct
has a pure virtual member function.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_CXXRecord_isAbstract(C: { _type: 'CXCursor' }): number {
  return addon.clang_CXXRecord_isAbstract(C);
}

/**
 * Determine if an enum declaration refers to a scoped enum.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_EnumDecl_isScoped(C: { _type: 'CXCursor' }): number {
  return addon.clang_EnumDecl_isScoped(C);
}

/**
 * Determine if a C++ member function or member function template is
declared 'const'.
 * @param C - CXCursor
 * @returns unsigned int
 */
export function clang_CXXMethod_isConst(C: { _type: 'CXCursor' }): number {
  return addon.clang_CXXMethod_isConst(C);
}

/**
 * Given a cursor that represents a template, determine
the cursor kind of the specializations would be generated by instantiating
the template.

This routine can be used to determine what flavor of function template,
class template, or class template partial specialization is stored in the
cursor. For example, it can describe whether a class template cursor is
declared with "struct", "class" or "union".

\param C The cursor to query. This cursor should represent a template
declaration.

\returns The cursor kind of the specializations that would be generated
by instantiating the template \p C. If \p C is not a template, returns
\c CXCursor_NoDeclFound.
 * @param C - CXCursor
 * @returns enum CXCursorKind
 */
export function clang_getTemplateCursorKind(C: { _type: 'CXCursor' }): number {
  return addon.clang_getTemplateCursorKind(C);
}

/**
 * Given a cursor that may represent a specialization or instantiation
of a template, retrieve the cursor that represents the template that it
specializes or from which it was instantiated.

This routine determines the template involved both for explicit
specializations of templates and for implicit instantiations of the template,
both of which are referred to as "specializations". For a class template
specialization (e.g., \c std::vector<bool>), this routine will return
either the primary template (\c std::vector) or, if the specialization was
instantiated from a class template partial specialization, the class template
partial specialization. For a class template partial specialization and a
function template specialization (including instantiations), this
this routine will return the specialized template.

For members of a class template (e.g., member functions, member classes, or
static data members), returns the specialized or instantiated member.
Although not strictly "templates" in the C++ language, members of class
templates have the same notions of specializations and instantiations that
templates do, so this routine treats them similarly.

\param C A cursor that may be a specialization of a template or a member
of a template.

\returns If the given cursor is a specialization or instantiation of a
template or a member thereof, the template or member that it specializes or
from which it was instantiated. Otherwise, returns a NULL cursor.
 * @param C - CXCursor
 * @returns CXCursor
 */
export function clang_getSpecializedCursorTemplate(C: { _type: 'CXCursor' }): { _type: 'CXCursor' } {
  return addon.clang_getSpecializedCursorTemplate(C);
}

/**
 * Given a cursor that references something else, return the source range
covering that reference.

\param C A cursor pointing to a member reference, a declaration reference, or
an operator call.
\param NameFlags A bitset with three independent flags:
CXNameRange_WantQualifier, CXNameRange_WantTemplateArgs, and
CXNameRange_WantSinglePiece.
\param PieceIndex For contiguous names or when passing the flag
CXNameRange_WantSinglePiece, only one piece with index 0 is
available. When the CXNameRange_WantSinglePiece flag is not passed for a
non-contiguous names, this index can be used to retrieve the individual
pieces of the name. See also CXNameRange_WantSinglePiece.

\returns The piece of the name pointed to by the given cursor. If there is no
name, or if the PieceIndex is out-of-range, a null-cursor will be returned.
 * @param C - CXCursor
 * @param NameFlags - unsigned int
 * @param PieceIndex - unsigned int
 * @returns CXSourceRange
 */
export function clang_getCursorReferenceNameRange(C: { _type: 'CXCursor' }, NameFlags: number, PieceIndex: number): { _type: 'CXSourceRange' } {
  return addon.clang_getCursorReferenceNameRange(C, NameFlags, PieceIndex);
}

/**
 * Get the raw lexical token starting with the given location.

\param TU the translation unit whose text is being tokenized.

\param Location the source location with which the token starts.

\returns The token starting with the given location or NULL if no such token
exist. The returned pointer must be freed with clang_disposeTokens before the
translation unit is destroyed.
 * @param TU - CXTranslationUnit
 * @param Location - CXSourceLocation
 * @returns CXToken *
 */
export function clang_getToken(TU: { _type: 'CXTranslationUnit' }, Location: { _type: 'CXSourceLocation' }): { _ptr: unknown } {
  return addon.clang_getToken(TU, Location);
}

/**
 * Determine the kind of the given token.
 * @param arg - CXToken
 * @returns CXTokenKind
 */
export function clang_getTokenKind(arg0: { _type: 'CXToken' }): { _type: 'CXTokenKind' } {
  return addon.clang_getTokenKind(arg0);
}

/**
 * Determine the spelling of the given token.

The spelling of a token is the textual representation of that token, e.g.,
the text of an identifier or keyword.
 * @param arg - CXTranslationUnit
 * @param arg - CXToken
 * @returns CXString
 */
export function clang_getTokenSpelling(arg0: { _type: 'CXTranslationUnit' }, arg1: { _type: 'CXToken' }): { _type: 'CXString' } {
  return addon.clang_getTokenSpelling(arg0, arg1);
}

/**
 * Retrieve the source location of the given token.
 * @param arg - CXTranslationUnit
 * @param arg - CXToken
 * @returns CXSourceLocation
 */
export function clang_getTokenLocation(arg0: { _type: 'CXTranslationUnit' }, arg1: { _type: 'CXToken' }): { _type: 'CXSourceLocation' } {
  return addon.clang_getTokenLocation(arg0, arg1);
}

/**
 * Retrieve a source range that covers the given token.
 * @param arg - CXTranslationUnit
 * @param arg - CXToken
 * @returns CXSourceRange
 */
export function clang_getTokenExtent(arg0: { _type: 'CXTranslationUnit' }, arg1: { _type: 'CXToken' }): { _type: 'CXSourceRange' } {
  return addon.clang_getTokenExtent(arg0, arg1);
}

/**
 * Tokenize the source code described by the given range into raw
lexical tokens.

\param TU the translation unit whose text is being tokenized.

\param Range the source range in which text should be tokenized. All of the
tokens produced by tokenization will fall within this source range,

\param Tokens this pointer will be set to point to the array of tokens
that occur within the given source range. The returned pointer must be
freed with clang_disposeTokens() before the translation unit is destroyed.

\param NumTokens will be set to the number of tokens in the \c *Tokens
array.
 * @param TU - CXTranslationUnit
 * @param Range - CXSourceRange
 * @param Tokens - CXToken **
 * @param NumTokens - unsigned int *
 */
export function clang_tokenize(TU: { _type: 'CXTranslationUnit' }, Range: { _type: 'CXSourceRange' }, Tokens: { _ptr: unknown }, NumTokens: { _ptr: unknown }): void {
  addon.clang_tokenize(TU, Range, Tokens, NumTokens);
}

/**
 * Annotate the given set of tokens by providing cursors for each token
that can be mapped to a specific entity within the abstract syntax tree.

This token-annotation routine is equivalent to invoking
clang_getCursor() for the source locations of each of the
tokens. The cursors provided are filtered, so that only those
cursors that have a direct correspondence to the token are
accepted. For example, given a function call \c f(x),
clang_getCursor() would provide the following cursors:

* when the cursor is over the 'f', a DeclRefExpr cursor referring to 'f'.
* when the cursor is over the '(' or the ')', a CallExpr referring to 'f'.
* when the cursor is over the 'x', a DeclRefExpr cursor referring to 'x'.

Only the first and last of these cursors will occur within the
annotate, since the tokens "f" and "x' directly refer to a function
and a variable, respectively, but the parentheses are just a small
part of the full syntax of the function call expression, which is
not provided as an annotation.

\param TU the translation unit that owns the given tokens.

\param Tokens the set of tokens to annotate.

\param NumTokens the number of tokens in \p Tokens.

\param Cursors an array of \p NumTokens cursors, whose contents will be
replaced with the cursors corresponding to each token.
 * @param TU - CXTranslationUnit
 * @param Tokens - CXToken *
 * @param NumTokens - unsigned int
 * @param Cursors - CXCursor *
 */
export function clang_annotateTokens(TU: { _type: 'CXTranslationUnit' }, Tokens: { _ptr: unknown }, NumTokens: number, Cursors: { _ptr: unknown }): void {
  addon.clang_annotateTokens(TU, Tokens, NumTokens, Cursors);
}

/**
 * Free the given set of tokens.
 * @param TU - CXTranslationUnit
 * @param Tokens - CXToken *
 * @param NumTokens - unsigned int
 */
export function clang_disposeTokens(TU: { _type: 'CXTranslationUnit' }, Tokens: { _ptr: unknown }, NumTokens: number): void {
  addon.clang_disposeTokens(TU, Tokens, NumTokens);
}

/**
 * \defgroup CINDEX_DEBUG Debugging facilities

These routines are used for testing and debugging, only, and should not
be relied upon.

@{
 * @param Kind - enum CXCursorKind
 * @returns CXString
 */
export function clang_getCursorKindSpelling(Kind: number): { _type: 'CXString' } {
  return addon.clang_getCursorKindSpelling(Kind);
}

export function clang_getDefinitionSpellingAndExtent(arg0: { _type: 'CXCursor' }, startBuf: { _ptr: unknown }, endBuf: { _ptr: unknown }, startLine: { _ptr: unknown }, startColumn: { _ptr: unknown }, endLine: { _ptr: unknown }, endColumn: { _ptr: unknown }): void {
  addon.clang_getDefinitionSpellingAndExtent(arg0, startBuf, endBuf, startLine, startColumn, endLine, endColumn);
}

export function clang_enableStackTraces(): void {
  addon.clang_enableStackTraces();
}

export function clang_executeOnThread(fn: { _type: 'void (*)(void *)' }, user_data: unknown, stack_size: number): void {
  addon.clang_executeOnThread(fn, user_data, stack_size);
}

/**
 * Determine the kind of a particular chunk within a completion string.

\param completion_string the completion string to query.

\param chunk_number the 0-based index of the chunk in the completion string.

\returns the kind of the chunk at the index \c chunk_number.
 * @param completion_string - CXCompletionString
 * @param chunk_number - unsigned int
 * @returns enum CXCompletionChunkKind
 */
export function clang_getCompletionChunkKind(completion_string: { _type: 'CXCompletionString' }, chunk_number: number): number {
  return addon.clang_getCompletionChunkKind(completion_string, chunk_number);
}

/**
 * Retrieve the text associated with a particular chunk within a
completion string.

\param completion_string the completion string to query.

\param chunk_number the 0-based index of the chunk in the completion string.

\returns the text associated with the chunk at index \c chunk_number.
 * @param completion_string - CXCompletionString
 * @param chunk_number - unsigned int
 * @returns CXString
 */
export function clang_getCompletionChunkText(completion_string: { _type: 'CXCompletionString' }, chunk_number: number): { _type: 'CXString' } {
  return addon.clang_getCompletionChunkText(completion_string, chunk_number);
}

/**
 * Retrieve the completion string associated with a particular chunk
within a completion string.

\param completion_string the completion string to query.

\param chunk_number the 0-based index of the chunk in the completion string.

\returns the completion string associated with the chunk at index
\c chunk_number.
 * @param completion_string - CXCompletionString
 * @param chunk_number - unsigned int
 * @returns CXCompletionString
 */
export function clang_getCompletionChunkCompletionString(completion_string: { _type: 'CXCompletionString' }, chunk_number: number): { _type: 'CXCompletionString' } {
  return addon.clang_getCompletionChunkCompletionString(completion_string, chunk_number);
}

/**
 * Retrieve the number of chunks in the given code-completion string.
 * @param completion_string - CXCompletionString
 * @returns unsigned int
 */
export function clang_getNumCompletionChunks(completion_string: { _type: 'CXCompletionString' }): number {
  return addon.clang_getNumCompletionChunks(completion_string);
}

/**
 * Determine the priority of this code completion.

The priority of a code completion indicates how likely it is that this
particular completion is the completion that the user will select. The
priority is selected by various internal heuristics.

\param completion_string The completion string to query.

\returns The priority of this completion string. Smaller values indicate
higher-priority (more likely) completions.
 * @param completion_string - CXCompletionString
 * @returns unsigned int
 */
export function clang_getCompletionPriority(completion_string: { _type: 'CXCompletionString' }): number {
  return addon.clang_getCompletionPriority(completion_string);
}

/**
 * Determine the availability of the entity that this code-completion
string refers to.

\param completion_string The completion string to query.

\returns The availability of the completion string.
 * @param completion_string - CXCompletionString
 * @returns enum CXAvailabilityKind
 */
export function clang_getCompletionAvailability(completion_string: { _type: 'CXCompletionString' }): number {
  return addon.clang_getCompletionAvailability(completion_string);
}

/**
 * Retrieve the number of annotations associated with the given
completion string.

\param completion_string the completion string to query.

\returns the number of annotations associated with the given completion
string.
 * @param completion_string - CXCompletionString
 * @returns unsigned int
 */
export function clang_getCompletionNumAnnotations(completion_string: { _type: 'CXCompletionString' }): number {
  return addon.clang_getCompletionNumAnnotations(completion_string);
}

/**
 * Retrieve the annotation associated with the given completion string.

\param completion_string the completion string to query.

\param annotation_number the 0-based index of the annotation of the
completion string.

\returns annotation string associated with the completion at index
\c annotation_number, or a NULL string if that annotation is not available.
 * @param completion_string - CXCompletionString
 * @param annotation_number - unsigned int
 * @returns CXString
 */
export function clang_getCompletionAnnotation(completion_string: { _type: 'CXCompletionString' }, annotation_number: number): { _type: 'CXString' } {
  return addon.clang_getCompletionAnnotation(completion_string, annotation_number);
}

/**
 * Retrieve the parent context of the given completion string.

The parent context of a completion string is the semantic parent of
the declaration (if any) that the code completion represents. For example,
a code completion for an Objective-C method would have the method's class
or protocol as its context.

\param completion_string The code completion string whose parent is
being queried.

\param kind DEPRECATED: always set to CXCursor_NotImplemented if non-NULL.

\returns The name of the completion parent, e.g., "NSObject" if
the completion string represents a method in the NSObject class.
 * @param completion_string - CXCompletionString
 * @param kind - enum CXCursorKind *
 * @returns CXString
 */
export function clang_getCompletionParent(completion_string: { _type: 'CXCompletionString' }, kind: { _ptr: unknown }): { _type: 'CXString' } {
  return addon.clang_getCompletionParent(completion_string, kind);
}

/**
 * Retrieve the brief documentation comment attached to the declaration
that corresponds to the given completion string.
 * @param completion_string - CXCompletionString
 * @returns CXString
 */
export function clang_getCompletionBriefComment(completion_string: { _type: 'CXCompletionString' }): { _type: 'CXString' } {
  return addon.clang_getCompletionBriefComment(completion_string);
}

/**
 * Retrieve a completion string for an arbitrary declaration or macro
definition cursor.

\param cursor The cursor to query.

\returns A non-context-sensitive completion string for declaration and macro
definition cursors, or NULL for other kinds of cursors.
 * @param cursor - CXCursor
 * @returns CXCompletionString
 */
export function clang_getCursorCompletionString(cursor: { _type: 'CXCursor' }): { _type: 'CXCompletionString' } {
  return addon.clang_getCursorCompletionString(cursor);
}

/**
 * Retrieve the number of fix-its for the given completion index.

Calling this makes sense only if CXCodeComplete_IncludeCompletionsWithFixIts
option was set.

\param results The structure keeping all completion results

\param completion_index The index of the completion

\return The number of fix-its which must be applied before the completion at
completion_index can be applied
 * @param results - CXCodeCompleteResults *
 * @param completion_index - unsigned int
 * @returns unsigned int
 */
export function clang_getCompletionNumFixIts(results: { _ptr: unknown }, completion_index: number): number {
  return addon.clang_getCompletionNumFixIts(results, completion_index);
}

/**
 * Fix-its that *must* be applied before inserting the text for the
corresponding completion.

By default, clang_codeCompleteAt() only returns completions with empty
fix-its. Extra completions with non-empty fix-its should be explicitly
requested by setting CXCodeComplete_IncludeCompletionsWithFixIts.

For the clients to be able to compute position of the cursor after applying
fix-its, the following conditions are guaranteed to hold for
replacement_range of the stored fix-its:
- Ranges in the fix-its are guaranteed to never contain the completion
point (or identifier under completion point, if any) inside them, except
at the start or at the end of the range.
- If a fix-it range starts or ends with completion point (or starts or
ends after the identifier under completion point), it will contain at
least one character. It allows to unambiguously recompute completion
point after applying the fix-it.

The intuition is that provided fix-its change code around the identifier we
complete, but are not allowed to touch the identifier itself or the
completion point. One example of completions with corrections are the ones
replacing '.' with '->' and vice versa:

std::unique_ptr<std::vector<int>> vec_ptr;
In 'vec_ptr.^', one of the completions is 'push_back', it requires
replacing '.' with '->'.
In 'vec_ptr->^', one of the completions is 'release', it requires
replacing '->' with '.'.

\param results The structure keeping all completion results

\param completion_index The index of the completion

\param fixit_index The index of the fix-it for the completion at
completion_index

\param replacement_range The fix-it range that must be replaced before the
completion at completion_index can be applied

\returns The fix-it string that must replace the code at replacement_range
before the completion at completion_index can be applied
 * @param results - CXCodeCompleteResults *
 * @param completion_index - unsigned int
 * @param fixit_index - unsigned int
 * @param replacement_range - CXSourceRange *
 * @returns CXString
 */
export function clang_getCompletionFixIt(results: { _ptr: unknown }, completion_index: number, fixit_index: number, replacement_range: { _ptr: unknown }): { _type: 'CXString' } {
  return addon.clang_getCompletionFixIt(results, completion_index, fixit_index, replacement_range);
}

/**
 * Returns a default set of code-completion options that can be
passed to\c clang_codeCompleteAt().
 * @returns unsigned int
 */
export function clang_defaultCodeCompleteOptions(): number {
  return addon.clang_defaultCodeCompleteOptions();
}

/**
 * Perform code completion at a given location in a translation unit.

This function performs code completion at a particular file, line, and
column within source code, providing results that suggest potential
code snippets based on the context of the completion. The basic model
for code completion is that Clang will parse a complete source file,
performing syntax checking up to the location where code-completion has
been requested. At that point, a special code-completion token is passed
to the parser, which recognizes this token and determines, based on the
current location in the C/Objective-C/C++ grammar and the state of
semantic analysis, what completions to provide. These completions are
returned via a new \c CXCodeCompleteResults structure.

Code completion itself is meant to be triggered by the client when the
user types punctuation characters or whitespace, at which point the
code-completion location will coincide with the cursor. For example, if \c p
is a pointer, code-completion might be triggered after the "-" and then
after the ">" in \c p->. When the code-completion location is after the ">",
the completion results will provide, e.g., the members of the struct that
"p" points to. The client is responsible for placing the cursor at the
beginning of the token currently being typed, then filtering the results
based on the contents of the token. For example, when code-completing for
the expression \c p->get, the client should provide the location just after
the ">" (e.g., pointing at the "g") to this code-completion hook. Then, the
client can filter the results based on the current token text ("get"), only
showing those results that start with "get". The intent of this interface
is to separate the relatively high-latency acquisition of code-completion
results from the filtering of results on a per-character basis, which must
have a lower latency.

\param TU The translation unit in which code-completion should
occur. The source files for this translation unit need not be
completely up-to-date (and the contents of those source files may
be overridden via \p unsaved_files). Cursors referring into the
translation unit may be invalidated by this invocation.

\param complete_filename The name of the source file where code
completion should be performed. This filename may be any file
included in the translation unit.

\param complete_line The line at which code-completion should occur.

\param complete_column The column at which code-completion should occur.
Note that the column should point just after the syntactic construct that
initiated code completion, and not in the middle of a lexical token.

\param unsaved_files the Files that have not yet been saved to disk
but may be required for parsing or code completion, including the
contents of those files.  The contents and name of these files (as
specified by CXUnsavedFile) are copied when necessary, so the
client only needs to guarantee their validity until the call to
this function returns.

\param num_unsaved_files The number of unsaved file entries in \p
unsaved_files.

\param options Extra options that control the behavior of code
completion, expressed as a bitwise OR of the enumerators of the
CXCodeComplete_Flags enumeration. The
\c clang_defaultCodeCompleteOptions() function returns a default set
of code-completion options.

\returns If successful, a new \c CXCodeCompleteResults structure
containing code-completion results, which should eventually be
freed with \c clang_disposeCodeCompleteResults(). If code
completion fails, returns NULL.
 * @param TU - CXTranslationUnit
 * @param complete_filename - const char *
 * @param complete_line - unsigned int
 * @param complete_column - unsigned int
 * @param unsaved_files - struct CXUnsavedFile *
 * @param num_unsaved_files - unsigned int
 * @param options - unsigned int
 * @returns CXCodeCompleteResults *
 */
export function clang_codeCompleteAt(TU: { _type: 'CXTranslationUnit' }, complete_filename: string, complete_line: number, complete_column: number, unsaved_files: { _ptr: unknown }, num_unsaved_files: number, options: number): { _ptr: unknown } {
  return addon.clang_codeCompleteAt(TU, complete_filename, complete_line, complete_column, unsaved_files, num_unsaved_files, options);
}

/**
 * Sort the code-completion results in case-insensitive alphabetical
order.

\param Results The set of results to sort.
\param NumResults The number of results in \p Results.
 * @param Results - CXCompletionResult *
 * @param NumResults - unsigned int
 */
export function clang_sortCodeCompletionResults(Results: { _ptr: unknown }, NumResults: number): void {
  addon.clang_sortCodeCompletionResults(Results, NumResults);
}

/**
 * Free the given set of code-completion results.
 * @param Results - CXCodeCompleteResults *
 */
export function clang_disposeCodeCompleteResults(Results: { _ptr: unknown }): void {
  addon.clang_disposeCodeCompleteResults(Results);
}

/**
 * Determine the number of diagnostics produced prior to the
location where code completion was performed.
 * @param Results - CXCodeCompleteResults *
 * @returns unsigned int
 */
export function clang_codeCompleteGetNumDiagnostics(Results: { _ptr: unknown }): number {
  return addon.clang_codeCompleteGetNumDiagnostics(Results);
}

/**
 * Retrieve a diagnostic associated with the given code completion.

\param Results the code completion results to query.
\param Index the zero-based diagnostic number to retrieve.

\returns the requested diagnostic. This diagnostic must be freed
via a call to \c clang_disposeDiagnostic().
 * @param Results - CXCodeCompleteResults *
 * @param Index - unsigned int
 * @returns CXDiagnostic
 */
export function clang_codeCompleteGetDiagnostic(Results: { _ptr: unknown }, Index: number): { _type: 'CXDiagnostic' } {
  return addon.clang_codeCompleteGetDiagnostic(Results, Index);
}

/**
 * Determines what completions are appropriate for the context
the given code completion.

\param Results the code completion results to query

\returns the kinds of completions that are appropriate for use
along with the given code completion results.
 * @param Results - CXCodeCompleteResults *
 * @returns unsigned long long
 */
export function clang_codeCompleteGetContexts(Results: { _ptr: unknown }): bigint {
  return addon.clang_codeCompleteGetContexts(Results);
}

/**
 * Returns the cursor kind for the container for the current code
completion context. The container is only guaranteed to be set for
contexts where a container exists (i.e. member accesses or Objective-C
message sends); if there is not a container, this function will return
CXCursor_InvalidCode.

\param Results the code completion results to query

\param IsIncomplete on return, this value will be false if Clang has complete
information about the container. If Clang does not have complete
information, this value will be true.

\returns the container kind, or CXCursor_InvalidCode if there is not a
container
 * @param Results - CXCodeCompleteResults *
 * @param IsIncomplete - unsigned int *
 * @returns enum CXCursorKind
 */
export function clang_codeCompleteGetContainerKind(Results: { _ptr: unknown }, IsIncomplete: { _ptr: unknown }): number {
  return addon.clang_codeCompleteGetContainerKind(Results, IsIncomplete);
}

/**
 * Returns the USR for the container for the current code completion
context. If there is not a container for the current context, this
function will return the empty string.

\param Results the code completion results to query

\returns the USR for the container
 * @param Results - CXCodeCompleteResults *
 * @returns CXString
 */
export function clang_codeCompleteGetContainerUSR(Results: { _ptr: unknown }): { _type: 'CXString' } {
  return addon.clang_codeCompleteGetContainerUSR(Results);
}

/**
 * Returns the currently-entered selector for an Objective-C message
send, formatted like "initWithFoo:bar:". Only guaranteed to return a
non-empty string for CXCompletionContext_ObjCInstanceMessage and
CXCompletionContext_ObjCClassMessage.

\param Results the code completion results to query

\returns the selector (or partial selector) that has been entered thus far
for an Objective-C message send.
 * @param Results - CXCodeCompleteResults *
 * @returns CXString
 */
export function clang_codeCompleteGetObjCSelector(Results: { _ptr: unknown }): { _type: 'CXString' } {
  return addon.clang_codeCompleteGetObjCSelector(Results);
}

/**
 * Return a version string, suitable for showing to a user, but not
intended to be parsed (the format is not guaranteed to be stable).
 * @returns CXString
 */
export function clang_getClangVersion(): { _type: 'CXString' } {
  return addon.clang_getClangVersion();
}

/**
 * Enable/disable crash recovery.

\param isEnabled Flag to indicate if crash recovery is enabled.  A non-zero
value enables crash recovery, while 0 disables it.
 * @param isEnabled - unsigned int
 */
export function clang_toggleCrashRecovery(isEnabled: number): void {
  addon.clang_toggleCrashRecovery(isEnabled);
}

/**
 * Visit the set of preprocessor inclusions in a translation unit.
The visitor function is called with the provided data for every included
file.  This does not include headers included by the PCH file (unless one
is inspecting the inclusions in the PCH file itself).
 * @param tu - CXTranslationUnit
 * @param visitor - CXInclusionVisitor
 * @param client_data - CXClientData
 */
export function clang_getInclusions(tu: { _type: 'CXTranslationUnit' }, visitor: { _type: 'CXInclusionVisitor' }, client_data: { _type: 'CXClientData' }): void {
  addon.clang_getInclusions(tu, visitor, client_data);
}

/**
 * If cursor is a statement declaration tries to evaluate the
statement and if its variable, tries to evaluate its initializer,
into its corresponding type.
If it's an expression, tries to evaluate the expression.
 * @param C - CXCursor
 * @returns CXEvalResult
 */
export function clang_Cursor_Evaluate(C: { _type: 'CXCursor' }): { _type: 'CXEvalResult' } {
  return addon.clang_Cursor_Evaluate(C);
}

/**
 * Returns the kind of the evaluated result.
 * @param E - CXEvalResult
 * @returns CXEvalResultKind
 */
export function clang_EvalResult_getKind(E: { _type: 'CXEvalResult' }): { _type: 'CXEvalResultKind' } {
  return addon.clang_EvalResult_getKind(E);
}

/**
 * Returns the evaluation result as integer if the
kind is Int.
 * @param E - CXEvalResult
 * @returns int
 */
export function clang_EvalResult_getAsInt(E: { _type: 'CXEvalResult' }): number {
  return addon.clang_EvalResult_getAsInt(E);
}

/**
 * Returns the evaluation result as a long long integer if the
kind is Int. This prevents overflows that may happen if the result is
returned with clang_EvalResult_getAsInt.
 * @param E - CXEvalResult
 * @returns long long
 */
export function clang_EvalResult_getAsLongLong(E: { _type: 'CXEvalResult' }): bigint {
  return addon.clang_EvalResult_getAsLongLong(E);
}

/**
 * Returns a non-zero value if the kind is Int and the evaluation
result resulted in an unsigned integer.
 * @param E - CXEvalResult
 * @returns unsigned int
 */
export function clang_EvalResult_isUnsignedInt(E: { _type: 'CXEvalResult' }): number {
  return addon.clang_EvalResult_isUnsignedInt(E);
}

/**
 * Returns the evaluation result as an unsigned integer if
the kind is Int and clang_EvalResult_isUnsignedInt is non-zero.
 * @param E - CXEvalResult
 * @returns unsigned long long
 */
export function clang_EvalResult_getAsUnsigned(E: { _type: 'CXEvalResult' }): bigint {
  return addon.clang_EvalResult_getAsUnsigned(E);
}

/**
 * Returns the evaluation result as double if the
kind is double.
 * @param E - CXEvalResult
 * @returns double
 */
export function clang_EvalResult_getAsDouble(E: { _type: 'CXEvalResult' }): number {
  return addon.clang_EvalResult_getAsDouble(E);
}

/**
 * Returns the evaluation result as a constant string if the
kind is other than Int or float. User must not free this pointer,
instead call clang_EvalResult_dispose on the CXEvalResult returned
by clang_Cursor_Evaluate.
 * @param E - CXEvalResult
 * @returns const char *
 */
export function clang_EvalResult_getAsStr(E: { _type: 'CXEvalResult' }): string {
  return addon.clang_EvalResult_getAsStr(E);
}

/**
 * Disposes the created Eval memory.
 * @param E - CXEvalResult
 */
export function clang_EvalResult_dispose(E: { _type: 'CXEvalResult' }): void {
  addon.clang_EvalResult_dispose(E);
}

/**
 * Retrieve a remapping.

\param path the path that contains metadata about remappings.

\returns the requested remapping. This remapping must be freed
via a call to \c clang_remap_dispose(). Can return NULL if an error occurred.
 * @param path - const char *
 * @returns CXRemapping
 */
export function clang_getRemappings(path: string): { _type: 'CXRemapping' } {
  return addon.clang_getRemappings(path);
}

/**
 * Retrieve a remapping.

\param filePaths pointer to an array of file paths containing remapping info.

\param numFiles number of file paths.

\returns the requested remapping. This remapping must be freed
via a call to \c clang_remap_dispose(). Can return NULL if an error occurred.
 * @param filePaths - const char **
 * @param numFiles - unsigned int
 * @returns CXRemapping
 */
export function clang_getRemappingsFromFileList(filePaths: { _ptr: unknown }, numFiles: number): { _type: 'CXRemapping' } {
  return addon.clang_getRemappingsFromFileList(filePaths, numFiles);
}

/**
 * Determine the number of remappings.
 * @param arg - CXRemapping
 * @returns unsigned int
 */
export function clang_remap_getNumFiles(arg0: { _type: 'CXRemapping' }): number {
  return addon.clang_remap_getNumFiles(arg0);
}

/**
 * Get the original and the associated filename from the remapping.

\param original If non-NULL, will be set to the original filename.

\param transformed If non-NULL, will be set to the filename that the original
is associated with.
 * @param arg - CXRemapping
 * @param index - unsigned int
 * @param original - CXString *
 * @param transformed - CXString *
 */
export function clang_remap_getFilenames(arg0: { _type: 'CXRemapping' }, index: number, original: { _ptr: unknown }, transformed: { _ptr: unknown }): void {
  addon.clang_remap_getFilenames(arg0, index, original, transformed);
}

/**
 * Dispose the remapping.
 * @param arg - CXRemapping
 */
export function clang_remap_dispose(arg0: { _type: 'CXRemapping' }): void {
  addon.clang_remap_dispose(arg0);
}

/**
 * Find references of a declaration in a specific file.

\param cursor pointing to a declaration or a reference of one.

\param file to search for references.

\param visitor callback that will receive pairs of CXCursor/CXSourceRange for
each reference found.
The CXSourceRange will point inside the file; if the reference is inside
a macro (and not a macro argument) the CXSourceRange will be invalid.

\returns one of the CXResult enumerators.
 * @param cursor - CXCursor
 * @param file - CXFile
 * @param visitor - CXCursorAndRangeVisitor
 * @returns CXResult
 */
export function clang_findReferencesInFile(cursor: { _type: 'CXCursor' }, file: { _type: 'CXFile' }, visitor: { _type: 'CXCursorAndRangeVisitor' }): { _type: 'CXResult' } {
  return addon.clang_findReferencesInFile(cursor, file, visitor);
}

/**
 * Find #import/#include directives in a specific file.

\param TU translation unit containing the file to query.

\param file to search for #import/#include directives.

\param visitor callback that will receive pairs of CXCursor/CXSourceRange for
each directive found.

\returns one of the CXResult enumerators.
 * @param TU - CXTranslationUnit
 * @param file - CXFile
 * @param visitor - CXCursorAndRangeVisitor
 * @returns CXResult
 */
export function clang_findIncludesInFile(TU: { _type: 'CXTranslationUnit' }, file: { _type: 'CXFile' }, visitor: { _type: 'CXCursorAndRangeVisitor' }): { _type: 'CXResult' } {
  return addon.clang_findIncludesInFile(TU, file, visitor);
}

export function clang_findReferencesInFileWithBlock(arg0: { _type: 'CXCursor' }, arg1: { _type: 'CXFile' }, arg2: { _type: 'CXCursorAndRangeVisitorBlock' }): { _type: 'CXResult' } {
  return addon.clang_findReferencesInFileWithBlock(arg0, arg1, arg2);
}

export function clang_findIncludesInFileWithBlock(arg0: { _type: 'CXTranslationUnit' }, arg1: { _type: 'CXFile' }, arg2: { _type: 'CXCursorAndRangeVisitorBlock' }): { _type: 'CXResult' } {
  return addon.clang_findIncludesInFileWithBlock(arg0, arg1, arg2);
}

export function clang_index_isEntityObjCContainerKind(arg0: { _type: 'CXIdxEntityKind' }): number {
  return addon.clang_index_isEntityObjCContainerKind(arg0);
}

export function clang_index_getObjCContainerDeclInfo(arg0: { _ptr: unknown }): { _ptr: unknown } {
  return addon.clang_index_getObjCContainerDeclInfo(arg0);
}

export function clang_index_getObjCInterfaceDeclInfo(arg0: { _ptr: unknown }): { _ptr: unknown } {
  return addon.clang_index_getObjCInterfaceDeclInfo(arg0);
}

export function clang_index_getObjCCategoryDeclInfo(arg0: { _ptr: unknown }): { _ptr: unknown } {
  return addon.clang_index_getObjCCategoryDeclInfo(arg0);
}

export function clang_index_getObjCProtocolRefListInfo(arg0: { _ptr: unknown }): { _ptr: unknown } {
  return addon.clang_index_getObjCProtocolRefListInfo(arg0);
}

export function clang_index_getObjCPropertyDeclInfo(arg0: { _ptr: unknown }): { _ptr: unknown } {
  return addon.clang_index_getObjCPropertyDeclInfo(arg0);
}

export function clang_index_getIBOutletCollectionAttrInfo(arg0: { _ptr: unknown }): { _ptr: unknown } {
  return addon.clang_index_getIBOutletCollectionAttrInfo(arg0);
}

export function clang_index_getCXXClassDeclInfo(arg0: { _ptr: unknown }): { _ptr: unknown } {
  return addon.clang_index_getCXXClassDeclInfo(arg0);
}

/**
 * For retrieving a custom CXIdxClientContainer attached to a
container.
 * @param arg - const CXIdxContainerInfo *
 * @returns CXIdxClientContainer
 */
export function clang_index_getClientContainer(arg0: { _ptr: unknown }): { _type: 'CXIdxClientContainer' } {
  return addon.clang_index_getClientContainer(arg0);
}

/**
 * For setting a custom CXIdxClientContainer attached to a
container.
 * @param arg - const CXIdxContainerInfo *
 * @param arg - CXIdxClientContainer
 */
export function clang_index_setClientContainer(arg0: { _ptr: unknown }, arg1: { _type: 'CXIdxClientContainer' }): void {
  addon.clang_index_setClientContainer(arg0, arg1);
}

/**
 * For retrieving a custom CXIdxClientEntity attached to an entity.
 * @param arg - const CXIdxEntityInfo *
 * @returns CXIdxClientEntity
 */
export function clang_index_getClientEntity(arg0: { _ptr: unknown }): { _type: 'CXIdxClientEntity' } {
  return addon.clang_index_getClientEntity(arg0);
}

/**
 * For setting a custom CXIdxClientEntity attached to an entity.
 * @param arg - const CXIdxEntityInfo *
 * @param arg - CXIdxClientEntity
 */
export function clang_index_setClientEntity(arg0: { _ptr: unknown }, arg1: { _type: 'CXIdxClientEntity' }): void {
  addon.clang_index_setClientEntity(arg0, arg1);
}

/**
 * An indexing action/session, to be applied to one or multiple
translation units.

\param CIdx The index object with which the index action will be associated.
 * @param CIdx - CXIndex
 * @returns CXIndexAction
 */
export function clang_IndexAction_create(CIdx: { _type: 'CXIndex' }): { _type: 'CXIndexAction' } {
  return addon.clang_IndexAction_create(CIdx);
}

/**
 * Destroy the given index action.

The index action must not be destroyed until all of the translation units
created within that index action have been destroyed.
 * @param arg - CXIndexAction
 */
export function clang_IndexAction_dispose(arg0: { _type: 'CXIndexAction' }): void {
  addon.clang_IndexAction_dispose(arg0);
}

/**
 * Index the given source file and the translation unit corresponding
to that file via callbacks implemented through #IndexerCallbacks.

\param client_data pointer data supplied by the client, which will
be passed to the invoked callbacks.

\param index_callbacks Pointer to indexing callbacks that the client
implements.

\param index_callbacks_size Size of #IndexerCallbacks structure that gets
passed in index_callbacks.

\param index_options A bitmask of options that affects how indexing is
performed. This should be a bitwise OR of the CXIndexOpt_XXX flags.

\param[out] out_TU pointer to store a \c CXTranslationUnit that can be
reused after indexing is finished. Set to \c NULL if you do not require it.

\returns 0 on success or if there were errors from which the compiler could
recover.  If there is a failure from which there is no recovery, returns
a non-zero \c CXErrorCode.

The rest of the parameters are the same as #clang_parseTranslationUnit.
 * @param arg - CXIndexAction
 * @param client_data - CXClientData
 * @param index_callbacks - IndexerCallbacks *
 * @param index_callbacks_size - unsigned int
 * @param index_options - unsigned int
 * @param source_filename - const char *
 * @param command_line_args - const char *const *
 * @param num_command_line_args - int
 * @param unsaved_files - struct CXUnsavedFile *
 * @param num_unsaved_files - unsigned int
 * @param out_TU - CXTranslationUnit *
 * @param TU_options - unsigned int
 * @returns int
 */
export function clang_indexSourceFile(arg0: { _type: 'CXIndexAction' }, client_data: { _type: 'CXClientData' }, index_callbacks: { _ptr: unknown }, index_callbacks_size: number, index_options: number, source_filename: string, command_line_args: { _ptr: unknown }, num_command_line_args: number, unsaved_files: { _ptr: unknown }, num_unsaved_files: number, out_TU: { _ptr: unknown }, TU_options: number): number {
  return addon.clang_indexSourceFile(arg0, client_data, index_callbacks, index_callbacks_size, index_options, source_filename, command_line_args, num_command_line_args, unsaved_files, num_unsaved_files, out_TU, TU_options);
}

/**
 * Same as clang_indexSourceFile but requires a full command line
for \c command_line_args including argv[0]. This is useful if the standard
library paths are relative to the binary.
 * @param arg - CXIndexAction
 * @param client_data - CXClientData
 * @param index_callbacks - IndexerCallbacks *
 * @param index_callbacks_size - unsigned int
 * @param index_options - unsigned int
 * @param source_filename - const char *
 * @param command_line_args - const char *const *
 * @param num_command_line_args - int
 * @param unsaved_files - struct CXUnsavedFile *
 * @param num_unsaved_files - unsigned int
 * @param out_TU - CXTranslationUnit *
 * @param TU_options - unsigned int
 * @returns int
 */
export function clang_indexSourceFileFullArgv(arg0: { _type: 'CXIndexAction' }, client_data: { _type: 'CXClientData' }, index_callbacks: { _ptr: unknown }, index_callbacks_size: number, index_options: number, source_filename: string, command_line_args: { _ptr: unknown }, num_command_line_args: number, unsaved_files: { _ptr: unknown }, num_unsaved_files: number, out_TU: { _ptr: unknown }, TU_options: number): number {
  return addon.clang_indexSourceFileFullArgv(arg0, client_data, index_callbacks, index_callbacks_size, index_options, source_filename, command_line_args, num_command_line_args, unsaved_files, num_unsaved_files, out_TU, TU_options);
}

/**
 * Index the given translation unit via callbacks implemented through
#IndexerCallbacks.

The order of callback invocations is not guaranteed to be the same as
when indexing a source file. The high level order will be:

-Preprocessor callbacks invocations
-Declaration/reference callbacks invocations
-Diagnostic callback invocations

The parameters are the same as #clang_indexSourceFile.

\returns If there is a failure from which there is no recovery, returns
non-zero, otherwise returns 0.
 * @param arg - CXIndexAction
 * @param client_data - CXClientData
 * @param index_callbacks - IndexerCallbacks *
 * @param index_callbacks_size - unsigned int
 * @param index_options - unsigned int
 * @param arg - CXTranslationUnit
 * @returns int
 */
export function clang_indexTranslationUnit(arg0: { _type: 'CXIndexAction' }, client_data: { _type: 'CXClientData' }, index_callbacks: { _ptr: unknown }, index_callbacks_size: number, index_options: number, arg5: { _type: 'CXTranslationUnit' }): number {
  return addon.clang_indexTranslationUnit(arg0, client_data, index_callbacks, index_callbacks_size, index_options, arg5);
}

/**
 * Retrieve the CXIdxFile, file, line, column, and offset represented by
the given CXIdxLoc.

If the location refers into a macro expansion, retrieves the
location of the macro expansion and if it refers into a macro argument
retrieves the location of the argument.
 * @param loc - CXIdxLoc
 * @param indexFile - CXIdxClientFile *
 * @param file - CXFile *
 * @param line - unsigned int *
 * @param column - unsigned int *
 * @param offset - unsigned int *
 */
export function clang_indexLoc_getFileLocation(loc: { _type: 'CXIdxLoc' }, indexFile: { _ptr: unknown }, file: { _ptr: unknown }, line: { _ptr: unknown }, column: { _ptr: unknown }, offset: { _ptr: unknown }): void {
  addon.clang_indexLoc_getFileLocation(loc, indexFile, file, line, column, offset);
}

/**
 * Retrieve the CXSourceLocation represented by the given CXIdxLoc.
 * @param loc - CXIdxLoc
 * @returns CXSourceLocation
 */
export function clang_indexLoc_getCXSourceLocation(loc: { _type: 'CXIdxLoc' }): { _type: 'CXSourceLocation' } {
  return addon.clang_indexLoc_getCXSourceLocation(loc);
}

/**
 * Visit the fields of a particular type.

This function visits all the direct fields of the given cursor,
invoking the given \p visitor function with the cursors of each
visited field. The traversal may be ended prematurely, if
the visitor returns \c CXFieldVisit_Break.

\param T the record type whose field may be visited.

\param visitor the visitor function that will be invoked for each
field of \p T.

\param client_data pointer data supplied by the client, which will
be passed to the visitor each time it is invoked.

\returns a non-zero value if the traversal was terminated
prematurely by the visitor returning \c CXFieldVisit_Break.
 * @param T - CXType
 * @param visitor - CXFieldVisitor
 * @param client_data - CXClientData
 * @returns unsigned int
 */
export function clang_Type_visitFields(T: { _type: 'CXType' }, visitor: { _type: 'CXFieldVisitor' }, client_data: { _type: 'CXClientData' }): number {
  return addon.clang_Type_visitFields(T, visitor, client_data);
}

/**
 * Visit the base classes of a type.

This function visits all the direct base classes of a the given cursor,
invoking the given \p visitor function with the cursors of each
visited base. The traversal may be ended prematurely, if
the visitor returns \c CXFieldVisit_Break.

\param T the record type whose field may be visited.

\param visitor the visitor function that will be invoked for each
field of \p T.

\param client_data pointer data supplied by the client, which will
be passed to the visitor each time it is invoked.

\returns a non-zero value if the traversal was terminated
prematurely by the visitor returning \c CXFieldVisit_Break.
 * @param T - CXType
 * @param visitor - CXFieldVisitor
 * @param client_data - CXClientData
 * @returns unsigned int
 */
export function clang_visitCXXBaseClasses(T: { _type: 'CXType' }, visitor: { _type: 'CXFieldVisitor' }, client_data: { _type: 'CXClientData' }): number {
  return addon.clang_visitCXXBaseClasses(T, visitor, client_data);
}

/**
 * Retrieve the spelling of a given CXBinaryOperatorKind.
 * @param kind - enum CXBinaryOperatorKind
 * @returns CXString
 */
export function clang_getBinaryOperatorKindSpelling(kind: number): { _type: 'CXString' } {
  return addon.clang_getBinaryOperatorKindSpelling(kind);
}

/**
 * Retrieve the binary operator kind of this cursor.

If this cursor is not a binary operator then returns Invalid.
 * @param cursor - CXCursor
 * @returns enum CXBinaryOperatorKind
 */
export function clang_getCursorBinaryOperatorKind(cursor: { _type: 'CXCursor' }): number {
  return addon.clang_getCursorBinaryOperatorKind(cursor);
}

/**
 * Retrieve the spelling of a given CXUnaryOperatorKind.
 * @param kind - enum CXUnaryOperatorKind
 * @returns CXString
 */
export function clang_getUnaryOperatorKindSpelling(kind: number): { _type: 'CXString' } {
  return addon.clang_getUnaryOperatorKindSpelling(kind);
}

/**
 * Retrieve the unary operator kind of this cursor.

If this cursor is not a unary operator then returns Invalid.
 * @param cursor - CXCursor
 * @returns enum CXUnaryOperatorKind
 */
export function clang_getCursorUnaryOperatorKind(cursor: { _type: 'CXCursor' }): number {
  return addon.clang_getCursorUnaryOperatorKind(cursor);
}

/**
 * Retrieve the character data associated with the given string.

The returned data is a reference and not owned by the user. This data
is only valid while the `CXString` is valid. This function is similar
to `std::string::c_str()`.
 * @param string - CXString
 * @returns const char *
 */
export function clang_getCString(string: { _type: 'CXString' }): string {
  return addon.clang_getCString(string);
}

/**
 * Free the given string.
 * @param string - CXString
 */
export function clang_disposeString(string: { _type: 'CXString' }): void {
  addon.clang_disposeString(string);
}

/**
 * Free the given string set.
 * @param set - CXStringSet *
 */
export function clang_disposeStringSet(set: { _ptr: unknown }): void {
  addon.clang_disposeStringSet(set);
}

/**
 * Retrieve a NULL (invalid) source location.
 * @returns CXSourceLocation
 */
export function clang_getNullLocation(): { _type: 'CXSourceLocation' } {
  return addon.clang_getNullLocation();
}

/**
 * Determine whether two source locations, which must refer into
the same translation unit, refer to exactly the same point in the source
code.

\returns non-zero if the source locations refer to the same location, zero
if they refer to different locations.
 * @param loc1 - CXSourceLocation
 * @param loc2 - CXSourceLocation
 * @returns unsigned int
 */
export function clang_equalLocations(loc1: { _type: 'CXSourceLocation' }, loc2: { _type: 'CXSourceLocation' }): number {
  return addon.clang_equalLocations(loc1, loc2);
}

/**
 * Determine for two source locations if the first comes
strictly before the second one in the source code.

\returns non-zero if the first source location comes
strictly before the second one, zero otherwise.
 * @param loc1 - CXSourceLocation
 * @param loc2 - CXSourceLocation
 * @returns unsigned int
 */
export function clang_isBeforeInTranslationUnit(loc1: { _type: 'CXSourceLocation' }, loc2: { _type: 'CXSourceLocation' }): number {
  return addon.clang_isBeforeInTranslationUnit(loc1, loc2);
}

/**
 * Returns non-zero if the given source location is in a system header.
 * @param location - CXSourceLocation
 * @returns int
 */
export function clang_Location_isInSystemHeader(location: { _type: 'CXSourceLocation' }): number {
  return addon.clang_Location_isInSystemHeader(location);
}

/**
 * Returns non-zero if the given source location is in the main file of
the corresponding translation unit.
 * @param location - CXSourceLocation
 * @returns int
 */
export function clang_Location_isFromMainFile(location: { _type: 'CXSourceLocation' }): number {
  return addon.clang_Location_isFromMainFile(location);
}

/**
 * Retrieve a NULL (invalid) source range.
 * @returns CXSourceRange
 */
export function clang_getNullRange(): { _type: 'CXSourceRange' } {
  return addon.clang_getNullRange();
}

/**
 * Retrieve a source range given the beginning and ending source
locations.
 * @param begin - CXSourceLocation
 * @param end - CXSourceLocation
 * @returns CXSourceRange
 */
export function clang_getRange(begin: { _type: 'CXSourceLocation' }, end: { _type: 'CXSourceLocation' }): { _type: 'CXSourceRange' } {
  return addon.clang_getRange(begin, end);
}

/**
 * Determine whether two ranges are equivalent.

\returns non-zero if the ranges are the same, zero if they differ.
 * @param range1 - CXSourceRange
 * @param range2 - CXSourceRange
 * @returns unsigned int
 */
export function clang_equalRanges(range1: { _type: 'CXSourceRange' }, range2: { _type: 'CXSourceRange' }): number {
  return addon.clang_equalRanges(range1, range2);
}

/**
 * Returns non-zero if \p range is null.
 * @param range - CXSourceRange
 * @returns int
 */
export function clang_Range_isNull(range: { _type: 'CXSourceRange' }): number {
  return addon.clang_Range_isNull(range);
}

/**
 * Retrieve the file, line, column, and offset represented by
the given source location.

If the location refers into a macro expansion, retrieves the
location of the macro expansion.

\param location the location within a source file that will be decomposed
into its parts.

\param file [out] if non-NULL, will be set to the file to which the given
source location points.

\param line [out] if non-NULL, will be set to the line to which the given
source location points.

\param column [out] if non-NULL, will be set to the column to which the given
source location points.

\param offset [out] if non-NULL, will be set to the offset into the
buffer to which the given source location points.
 * @param location - CXSourceLocation
 * @param file - CXFile *
 * @param line - unsigned int *
 * @param column - unsigned int *
 * @param offset - unsigned int *
 */
export function clang_getExpansionLocation(location: { _type: 'CXSourceLocation' }, file: { _ptr: unknown }, line: { _ptr: unknown }, column: { _ptr: unknown }, offset: { _ptr: unknown }): void {
  addon.clang_getExpansionLocation(location, file, line, column, offset);
}

/**
 * Retrieve the file, line and column represented by the given source
location, as specified in a # line directive.

Example: given the following source code in a file somefile.c

\code
#123 "dummy.c" 1

static int func(void)
{
return 0;
}
\endcode

the location information returned by this function would be

File: dummy.c Line: 124 Column: 12

whereas clang_getExpansionLocation would have returned

File: somefile.c Line: 3 Column: 12

\param location the location within a source file that will be decomposed
into its parts.

\param filename [out] if non-NULL, will be set to the filename of the
source location. Note that filenames returned will be for "virtual" files,
which don't necessarily exist on the machine running clang - e.g. when
parsing preprocessed output obtained from a different environment. If
a non-NULL value is passed in, remember to dispose of the returned value
using \c clang_disposeString() once you've finished with it. For an invalid
source location, an empty string is returned.

\param line [out] if non-NULL, will be set to the line number of the
source location. For an invalid source location, zero is returned.

\param column [out] if non-NULL, will be set to the column number of the
source location. For an invalid source location, zero is returned.
 * @param location - CXSourceLocation
 * @param filename - CXString *
 * @param line - unsigned int *
 * @param column - unsigned int *
 */
export function clang_getPresumedLocation(location: { _type: 'CXSourceLocation' }, filename: { _ptr: unknown }, line: { _ptr: unknown }, column: { _ptr: unknown }): void {
  addon.clang_getPresumedLocation(location, filename, line, column);
}

/**
 * Legacy API to retrieve the file, line, column, and offset represented
by the given source location.

This interface has been replaced by the newer interface
#clang_getExpansionLocation(). See that interface's documentation for
details.
 * @param location - CXSourceLocation
 * @param file - CXFile *
 * @param line - unsigned int *
 * @param column - unsigned int *
 * @param offset - unsigned int *
 */
export function clang_getInstantiationLocation(location: { _type: 'CXSourceLocation' }, file: { _ptr: unknown }, line: { _ptr: unknown }, column: { _ptr: unknown }, offset: { _ptr: unknown }): void {
  addon.clang_getInstantiationLocation(location, file, line, column, offset);
}

/**
 * Retrieve the file, line, column, and offset represented by
the given source location.

If the location refers into a macro instantiation, return where the
location was originally spelled in the source file.

\param location the location within a source file that will be decomposed
into its parts.

\param file [out] if non-NULL, will be set to the file to which the given
source location points.

\param line [out] if non-NULL, will be set to the line to which the given
source location points.

\param column [out] if non-NULL, will be set to the column to which the given
source location points.

\param offset [out] if non-NULL, will be set to the offset into the
buffer to which the given source location points.
 * @param location - CXSourceLocation
 * @param file - CXFile *
 * @param line - unsigned int *
 * @param column - unsigned int *
 * @param offset - unsigned int *
 */
export function clang_getSpellingLocation(location: { _type: 'CXSourceLocation' }, file: { _ptr: unknown }, line: { _ptr: unknown }, column: { _ptr: unknown }, offset: { _ptr: unknown }): void {
  addon.clang_getSpellingLocation(location, file, line, column, offset);
}

/**
 * Retrieve the file, line, column, and offset represented by
the given source location.

If the location refers into a macro expansion, return where the macro was
expanded or where the macro argument was written, if the location points at
a macro argument.

\param location the location within a source file that will be decomposed
into its parts.

\param file [out] if non-NULL, will be set to the file to which the given
source location points.

\param line [out] if non-NULL, will be set to the line to which the given
source location points.

\param column [out] if non-NULL, will be set to the column to which the given
source location points.

\param offset [out] if non-NULL, will be set to the offset into the
buffer to which the given source location points.
 * @param location - CXSourceLocation
 * @param file - CXFile *
 * @param line - unsigned int *
 * @param column - unsigned int *
 * @param offset - unsigned int *
 */
export function clang_getFileLocation(location: { _type: 'CXSourceLocation' }, file: { _ptr: unknown }, line: { _ptr: unknown }, column: { _ptr: unknown }, offset: { _ptr: unknown }): void {
  addon.clang_getFileLocation(location, file, line, column, offset);
}

/**
 * Retrieve a source location representing the first character within a
source range.
 * @param range - CXSourceRange
 * @returns CXSourceLocation
 */
export function clang_getRangeStart(range: { _type: 'CXSourceRange' }): { _type: 'CXSourceLocation' } {
  return addon.clang_getRangeStart(range);
}

/**
 * Retrieve a source location representing the last character within a
source range.
 * @param range - CXSourceRange
 * @returns CXSourceLocation
 */
export function clang_getRangeEnd(range: { _type: 'CXSourceRange' }): { _type: 'CXSourceLocation' } {
  return addon.clang_getRangeEnd(range);
}

/**
 * Destroy the given \c CXSourceRangeList.
 * @param ranges - CXSourceRangeList *
 */
export function clang_disposeSourceRangeList(ranges: { _ptr: unknown }): void {
  addon.clang_disposeSourceRangeList(ranges);
}


// Struct helper functions
export function createCXTargetInfoImpl(init?: CXTargetInfoImplInit): CXTargetInfoImpl {
  return addon.create_CXTargetInfoImpl(init);
}

/**
 * Get all fields from CXTargetInfoImpl as an object
 */
export function getCXTargetInfoImplFields(struct: CXTargetInfoImpl): CXTargetInfoImplInit {
  return {
  };
}

export function createCXTranslationUnitImpl(init?: CXTranslationUnitImplInit): CXTranslationUnitImpl {
  return addon.create_CXTranslationUnitImpl(init);
}

/**
 * Get all fields from CXTranslationUnitImpl as an object
 */
export function getCXTranslationUnitImplFields(struct: CXTranslationUnitImpl): CXTranslationUnitImplInit {
  return {
  };
}

/**
 * Create a new CXUnsavedFile instance
 * Provides the contents of a file that has not yet been saved to disk.

Each CXUnsavedFile instance provides the name of a file on the
system along with the current contents of that file that have not
yet been saved to disk.
 */
export function createCXUnsavedFile(init?: CXUnsavedFileInit): CXUnsavedFile {
  return addon.create_CXUnsavedFile(init);
}

/**
 * Get Filename field from CXUnsavedFile
 */
export function getCXUnsavedFile_Filename(struct: CXUnsavedFile): string {
  return addon.get_CXUnsavedFile_field(struct, 'Filename');
}

/**
 * Get Contents field from CXUnsavedFile
 */
export function getCXUnsavedFile_Contents(struct: CXUnsavedFile): string {
  return addon.get_CXUnsavedFile_field(struct, 'Contents');
}

/**
 * Get Length field from CXUnsavedFile
 */
export function getCXUnsavedFile_Length(struct: CXUnsavedFile): number {
  return addon.get_CXUnsavedFile_field(struct, 'Length');
}

/**
 * Get all fields from CXUnsavedFile as an object
 */
export function getCXUnsavedFileFields(struct: CXUnsavedFile): CXUnsavedFileInit {
  return {
    Filename: addon.get_CXUnsavedFile_field(struct, 'Filename'),
    Contents: addon.get_CXUnsavedFile_field(struct, 'Contents'),
    Length: addon.get_CXUnsavedFile_field(struct, 'Length'),
  };
}

/**
 * Create a new CXVersion instance
 * Describes a version number of the form major.minor.subminor.
 */
export function createCXVersion(init?: CXVersionInit): CXVersion {
  return addon.create_CXVersion(init);
}

/**
 * Get Major field from CXVersion
 */
export function getCXVersion_Major(struct: CXVersion): number {
  return addon.get_CXVersion_field(struct, 'Major');
}

/**
 * Get Minor field from CXVersion
 */
export function getCXVersion_Minor(struct: CXVersion): number {
  return addon.get_CXVersion_field(struct, 'Minor');
}

/**
 * Get Subminor field from CXVersion
 */
export function getCXVersion_Subminor(struct: CXVersion): number {
  return addon.get_CXVersion_field(struct, 'Subminor');
}

/**
 * Get all fields from CXVersion as an object
 */
export function getCXVersionFields(struct: CXVersion): CXVersionInit {
  return {
    Major: addon.get_CXVersion_field(struct, 'Major'),
    Minor: addon.get_CXVersion_field(struct, 'Minor'),
    Subminor: addon.get_CXVersion_field(struct, 'Subminor'),
  };
}

/**
 * Create a new CXIndexOptions instance
 * Index initialization options.

0 is the default value of each member of this struct except for Size.
Initialize the struct in one of the following three ways to avoid adapting
code each time a new member is added to it:
\code
CXIndexOptions Opts;
memset(&Opts, 0, sizeof(Opts));
Opts.Size = sizeof(CXIndexOptions);
\endcode
or explicitly initialize the first data member and zero-initialize the rest:
\code
CXIndexOptions Opts = { sizeof(CXIndexOptions) };
\endcode
or to prevent the -Wmissing-field-initializers warning for the above version:
\code
CXIndexOptions Opts{};
Opts.Size = sizeof(CXIndexOptions);
\endcode
 */
export function createCXIndexOptions(init?: CXIndexOptionsInit): CXIndexOptions {
  return addon.create_CXIndexOptions(init);
}

/**
 * Get Size field from CXIndexOptions
 */
export function getCXIndexOptions_Size(struct: CXIndexOptions): number {
  return addon.get_CXIndexOptions_field(struct, 'Size');
}

/**
 * Get ThreadBackgroundPriorityForIndexing field from CXIndexOptions
 */
export function getCXIndexOptions_ThreadBackgroundPriorityForIndexing(struct: CXIndexOptions): number {
  return addon.get_CXIndexOptions_field(struct, 'ThreadBackgroundPriorityForIndexing');
}

/**
 * Get ThreadBackgroundPriorityForEditing field from CXIndexOptions
 */
export function getCXIndexOptions_ThreadBackgroundPriorityForEditing(struct: CXIndexOptions): number {
  return addon.get_CXIndexOptions_field(struct, 'ThreadBackgroundPriorityForEditing');
}

/**
 * Get ExcludeDeclarationsFromPCH field from CXIndexOptions
 */
export function getCXIndexOptions_ExcludeDeclarationsFromPCH(struct: CXIndexOptions): number {
  return addon.get_CXIndexOptions_field(struct, 'ExcludeDeclarationsFromPCH');
}

/**
 * Get DisplayDiagnostics field from CXIndexOptions
 */
export function getCXIndexOptions_DisplayDiagnostics(struct: CXIndexOptions): number {
  return addon.get_CXIndexOptions_field(struct, 'DisplayDiagnostics');
}

/**
 * Get StorePreamblesInMemory field from CXIndexOptions
 */
export function getCXIndexOptions_StorePreamblesInMemory(struct: CXIndexOptions): number {
  return addon.get_CXIndexOptions_field(struct, 'StorePreamblesInMemory');
}

/**
 * Get PreambleStoragePath field from CXIndexOptions
 */
export function getCXIndexOptions_PreambleStoragePath(struct: CXIndexOptions): string {
  return addon.get_CXIndexOptions_field(struct, 'PreambleStoragePath');
}

/**
 * Get InvocationEmissionPath field from CXIndexOptions
 */
export function getCXIndexOptions_InvocationEmissionPath(struct: CXIndexOptions): string {
  return addon.get_CXIndexOptions_field(struct, 'InvocationEmissionPath');
}

/**
 * Get all fields from CXIndexOptions as an object
 */
export function getCXIndexOptionsFields(struct: CXIndexOptions): CXIndexOptionsInit {
  return {
    Size: addon.get_CXIndexOptions_field(struct, 'Size'),
    ThreadBackgroundPriorityForIndexing: addon.get_CXIndexOptions_field(struct, 'ThreadBackgroundPriorityForIndexing'),
    ThreadBackgroundPriorityForEditing: addon.get_CXIndexOptions_field(struct, 'ThreadBackgroundPriorityForEditing'),
    ExcludeDeclarationsFromPCH: addon.get_CXIndexOptions_field(struct, 'ExcludeDeclarationsFromPCH'),
    DisplayDiagnostics: addon.get_CXIndexOptions_field(struct, 'DisplayDiagnostics'),
    StorePreamblesInMemory: addon.get_CXIndexOptions_field(struct, 'StorePreamblesInMemory'),
    PreambleStoragePath: addon.get_CXIndexOptions_field(struct, 'PreambleStoragePath'),
    InvocationEmissionPath: addon.get_CXIndexOptions_field(struct, 'InvocationEmissionPath'),
  };
}

export function createCXTUResourceUsageEntry(init?: CXTUResourceUsageEntryInit): CXTUResourceUsageEntry {
  return addon.create_CXTUResourceUsageEntry(init);
}

/**
 * Get kind field from CXTUResourceUsageEntry
 */
export function getCXTUResourceUsageEntry_kind(struct: CXTUResourceUsageEntry): number {
  return addon.get_CXTUResourceUsageEntry_field(struct, 'kind');
}

/**
 * Get amount field from CXTUResourceUsageEntry
 */
export function getCXTUResourceUsageEntry_amount(struct: CXTUResourceUsageEntry): number {
  return addon.get_CXTUResourceUsageEntry_field(struct, 'amount');
}

/**
 * Get all fields from CXTUResourceUsageEntry as an object
 */
export function getCXTUResourceUsageEntryFields(struct: CXTUResourceUsageEntry): CXTUResourceUsageEntryInit {
  return {
    kind: addon.get_CXTUResourceUsageEntry_field(struct, 'kind'),
    amount: addon.get_CXTUResourceUsageEntry_field(struct, 'amount'),
  };
}

/**
 * Create a new CXTUResourceUsage instance
 * The memory usage of a CXTranslationUnit, broken into categories.
 */
export function createCXTUResourceUsage(init?: CXTUResourceUsageInit): CXTUResourceUsage {
  return addon.create_CXTUResourceUsage(init);
}

/**
 * Get data field from CXTUResourceUsage
 */
export function getCXTUResourceUsage_data(struct: CXTUResourceUsage): unknown {
  return addon.get_CXTUResourceUsage_field(struct, 'data');
}

/**
 * Get numEntries field from CXTUResourceUsage
 */
export function getCXTUResourceUsage_numEntries(struct: CXTUResourceUsage): number {
  return addon.get_CXTUResourceUsage_field(struct, 'numEntries');
}

/**
 * Get entries field from CXTUResourceUsage
 */
export function getCXTUResourceUsage_entries(struct: CXTUResourceUsage): { _ptr: unknown } {
  return addon.get_CXTUResourceUsage_field(struct, 'entries');
}

/**
 * Get all fields from CXTUResourceUsage as an object
 */
export function getCXTUResourceUsageFields(struct: CXTUResourceUsage): CXTUResourceUsageInit {
  return {
    data: addon.get_CXTUResourceUsage_field(struct, 'data'),
    numEntries: addon.get_CXTUResourceUsage_field(struct, 'numEntries'),
    entries: addon.get_CXTUResourceUsage_field(struct, 'entries'),
  };
}

/**
 * Create a new CXCursor instance
 * A cursor representing some element in the abstract syntax tree for
a translation unit.

The cursor abstraction unifies the different kinds of entities in a
program--declaration, statements, expressions, references to declarations,
etc.--under a single "cursor" abstraction with a common set of operations.
Common operation for a cursor include: getting the physical location in
a source file where the cursor points, getting the name associated with a
cursor, and retrieving cursors for any child nodes of a particular cursor.

Cursors can be produced in two specific ways.
clang_getTranslationUnitCursor() produces a cursor for a translation unit,
from which one can use clang_visitChildren() to explore the rest of the
translation unit. clang_getCursor() maps from a physical source location
to the entity that resides at that location, allowing one to map from the
source code into the AST.
 */
export function createCXCursor(init?: CXCursorInit): CXCursor {
  return addon.create_CXCursor(init);
}

/**
 * Get kind field from CXCursor
 */
export function getCXCursor_kind(struct: CXCursor): number {
  return addon.get_CXCursor_field(struct, 'kind');
}

/**
 * Get xdata field from CXCursor
 */
export function getCXCursor_xdata(struct: CXCursor): number {
  return addon.get_CXCursor_field(struct, 'xdata');
}

/**
 * Get data field from CXCursor
 */
export function getCXCursor_data(struct: CXCursor): unknown[] {
  return addon.get_CXCursor_field(struct, 'data');
}

/**
 * Get all fields from CXCursor as an object
 */
export function getCXCursorFields(struct: CXCursor): CXCursorInit {
  return {
    kind: addon.get_CXCursor_field(struct, 'kind'),
    xdata: addon.get_CXCursor_field(struct, 'xdata'),
    data: addon.get_CXCursor_field(struct, 'data'),
  };
}

/**
 * Create a new CXPlatformAvailability instance
 * Describes the availability of a given entity on a particular platform, e.g.,
a particular class might only be available on Mac OS 10.7 or newer.
 */
export function createCXPlatformAvailability(init?: CXPlatformAvailabilityInit): CXPlatformAvailability {
  return addon.create_CXPlatformAvailability(init);
}

/**
 * Get Platform field from CXPlatformAvailability
 */
export function getCXPlatformAvailability_Platform(struct: CXPlatformAvailability): { _type: 'CXString' } {
  return addon.get_CXPlatformAvailability_field(struct, 'Platform');
}

/**
 * Get Introduced field from CXPlatformAvailability
 */
export function getCXPlatformAvailability_Introduced(struct: CXPlatformAvailability): { _type: 'CXVersion' } {
  return addon.get_CXPlatformAvailability_field(struct, 'Introduced');
}

/**
 * Get Deprecated field from CXPlatformAvailability
 */
export function getCXPlatformAvailability_Deprecated(struct: CXPlatformAvailability): { _type: 'CXVersion' } {
  return addon.get_CXPlatformAvailability_field(struct, 'Deprecated');
}

/**
 * Get Obsoleted field from CXPlatformAvailability
 */
export function getCXPlatformAvailability_Obsoleted(struct: CXPlatformAvailability): { _type: 'CXVersion' } {
  return addon.get_CXPlatformAvailability_field(struct, 'Obsoleted');
}

/**
 * Get Unavailable field from CXPlatformAvailability
 */
export function getCXPlatformAvailability_Unavailable(struct: CXPlatformAvailability): number {
  return addon.get_CXPlatformAvailability_field(struct, 'Unavailable');
}

/**
 * Get Message field from CXPlatformAvailability
 */
export function getCXPlatformAvailability_Message(struct: CXPlatformAvailability): { _type: 'CXString' } {
  return addon.get_CXPlatformAvailability_field(struct, 'Message');
}

/**
 * Get all fields from CXPlatformAvailability as an object
 */
export function getCXPlatformAvailabilityFields(struct: CXPlatformAvailability): CXPlatformAvailabilityInit {
  return {
    Platform: addon.get_CXPlatformAvailability_field(struct, 'Platform'),
    Introduced: addon.get_CXPlatformAvailability_field(struct, 'Introduced'),
    Deprecated: addon.get_CXPlatformAvailability_field(struct, 'Deprecated'),
    Obsoleted: addon.get_CXPlatformAvailability_field(struct, 'Obsoleted'),
    Unavailable: addon.get_CXPlatformAvailability_field(struct, 'Unavailable'),
    Message: addon.get_CXPlatformAvailability_field(struct, 'Message'),
  };
}

export function createCXCursorSetImpl(init?: CXCursorSetImplInit): CXCursorSetImpl {
  return addon.create_CXCursorSetImpl(init);
}

/**
 * Get all fields from CXCursorSetImpl as an object
 */
export function getCXCursorSetImplFields(struct: CXCursorSetImpl): CXCursorSetImplInit {
  return {
  };
}

/**
 * Create a new CXType instance
 * The type of an element in the abstract syntax tree.
 */
export function createCXType(init?: CXTypeInit): CXType {
  return addon.create_CXType(init);
}

/**
 * Get kind field from CXType
 */
export function getCXType_kind(struct: CXType): number {
  return addon.get_CXType_field(struct, 'kind');
}

/**
 * Get data field from CXType
 */
export function getCXType_data(struct: CXType): unknown[] {
  return addon.get_CXType_field(struct, 'data');
}

/**
 * Get all fields from CXType as an object
 */
export function getCXTypeFields(struct: CXType): CXTypeInit {
  return {
    kind: addon.get_CXType_field(struct, 'kind'),
    data: addon.get_CXType_field(struct, 'data'),
  };
}

/**
 * Create a new CXToken instance
 * Describes a single preprocessing token.
 */
export function createCXToken(init?: CXTokenInit): CXToken {
  return addon.create_CXToken(init);
}

/**
 * Get int_data field from CXToken
 */
export function getCXToken_int_data(struct: CXToken): unknown[] {
  return addon.get_CXToken_field(struct, 'int_data');
}

/**
 * Get ptr_data field from CXToken
 */
export function getCXToken_ptr_data(struct: CXToken): unknown {
  return addon.get_CXToken_field(struct, 'ptr_data');
}

/**
 * Get all fields from CXToken as an object
 */
export function getCXTokenFields(struct: CXToken): CXTokenInit {
  return {
    int_data: addon.get_CXToken_field(struct, 'int_data'),
    ptr_data: addon.get_CXToken_field(struct, 'ptr_data'),
  };
}

/**
 * Create a new CXCompletionResult instance
 * A single result of code completion.
 */
export function createCXCompletionResult(init?: CXCompletionResultInit): CXCompletionResult {
  return addon.create_CXCompletionResult(init);
}

/**
 * Get CursorKind field from CXCompletionResult
 */
export function getCXCompletionResult_CursorKind(struct: CXCompletionResult): number {
  return addon.get_CXCompletionResult_field(struct, 'CursorKind');
}

/**
 * Get CompletionString field from CXCompletionResult
 */
export function getCXCompletionResult_CompletionString(struct: CXCompletionResult): { _type: 'CXCompletionString' } {
  return addon.get_CXCompletionResult_field(struct, 'CompletionString');
}

/**
 * Get all fields from CXCompletionResult as an object
 */
export function getCXCompletionResultFields(struct: CXCompletionResult): CXCompletionResultInit {
  return {
    CursorKind: addon.get_CXCompletionResult_field(struct, 'CursorKind'),
    CompletionString: addon.get_CXCompletionResult_field(struct, 'CompletionString'),
  };
}

/**
 * Create a new CXCodeCompleteResults instance
 * Contains the results of code-completion.

This data structure contains the results of code completion, as
produced by \c clang_codeCompleteAt(). Its contents must be freed by
\c clang_disposeCodeCompleteResults.
 */
export function createCXCodeCompleteResults(init?: CXCodeCompleteResultsInit): CXCodeCompleteResults {
  return addon.create_CXCodeCompleteResults(init);
}

/**
 * Get Results field from CXCodeCompleteResults
 */
export function getCXCodeCompleteResults_Results(struct: CXCodeCompleteResults): { _ptr: unknown } {
  return addon.get_CXCodeCompleteResults_field(struct, 'Results');
}

/**
 * Get NumResults field from CXCodeCompleteResults
 */
export function getCXCodeCompleteResults_NumResults(struct: CXCodeCompleteResults): number {
  return addon.get_CXCodeCompleteResults_field(struct, 'NumResults');
}

/**
 * Get all fields from CXCodeCompleteResults as an object
 */
export function getCXCodeCompleteResultsFields(struct: CXCodeCompleteResults): CXCodeCompleteResultsInit {
  return {
    Results: addon.get_CXCodeCompleteResults_field(struct, 'Results'),
    NumResults: addon.get_CXCodeCompleteResults_field(struct, 'NumResults'),
  };
}

export function createCXCursorAndRangeVisitor(init?: CXCursorAndRangeVisitorInit): CXCursorAndRangeVisitor {
  return addon.create_CXCursorAndRangeVisitor(init);
}

/**
 * Get context field from CXCursorAndRangeVisitor
 */
export function getCXCursorAndRangeVisitor_context(struct: CXCursorAndRangeVisitor): unknown {
  return addon.get_CXCursorAndRangeVisitor_field(struct, 'context');
}

/**
 * Get visit field from CXCursorAndRangeVisitor
 */
export function getCXCursorAndRangeVisitor_visit(struct: CXCursorAndRangeVisitor): number {
  return addon.get_CXCursorAndRangeVisitor_field(struct, 'visit');
}

/**
 * Get all fields from CXCursorAndRangeVisitor as an object
 */
export function getCXCursorAndRangeVisitorFields(struct: CXCursorAndRangeVisitor): CXCursorAndRangeVisitorInit {
  return {
    context: addon.get_CXCursorAndRangeVisitor_field(struct, 'context'),
    visit: addon.get_CXCursorAndRangeVisitor_field(struct, 'visit'),
  };
}

/**
 * Create a new CXIdxLoc instance
 * Source location passed to index callbacks.
 */
export function createCXIdxLoc(init?: CXIdxLocInit): CXIdxLoc {
  return addon.create_CXIdxLoc(init);
}

/**
 * Get ptr_data field from CXIdxLoc
 */
export function getCXIdxLoc_ptr_data(struct: CXIdxLoc): unknown[] {
  return addon.get_CXIdxLoc_field(struct, 'ptr_data');
}

/**
 * Get int_data field from CXIdxLoc
 */
export function getCXIdxLoc_int_data(struct: CXIdxLoc): number {
  return addon.get_CXIdxLoc_field(struct, 'int_data');
}

/**
 * Get all fields from CXIdxLoc as an object
 */
export function getCXIdxLocFields(struct: CXIdxLoc): CXIdxLocInit {
  return {
    ptr_data: addon.get_CXIdxLoc_field(struct, 'ptr_data'),
    int_data: addon.get_CXIdxLoc_field(struct, 'int_data'),
  };
}

/**
 * Create a new CXIdxIncludedFileInfo instance
 * Data for ppIncludedFile callback.
 */
export function createCXIdxIncludedFileInfo(init?: CXIdxIncludedFileInfoInit): CXIdxIncludedFileInfo {
  return addon.create_CXIdxIncludedFileInfo(init);
}

/**
 * Get hashLoc field from CXIdxIncludedFileInfo
 */
export function getCXIdxIncludedFileInfo_hashLoc(struct: CXIdxIncludedFileInfo): { _type: 'CXIdxLoc' } {
  return addon.get_CXIdxIncludedFileInfo_field(struct, 'hashLoc');
}

/**
 * Get filename field from CXIdxIncludedFileInfo
 */
export function getCXIdxIncludedFileInfo_filename(struct: CXIdxIncludedFileInfo): string {
  return addon.get_CXIdxIncludedFileInfo_field(struct, 'filename');
}

/**
 * Get file field from CXIdxIncludedFileInfo
 */
export function getCXIdxIncludedFileInfo_file(struct: CXIdxIncludedFileInfo): { _type: 'CXFile' } {
  return addon.get_CXIdxIncludedFileInfo_field(struct, 'file');
}

/**
 * Get isImport field from CXIdxIncludedFileInfo
 */
export function getCXIdxIncludedFileInfo_isImport(struct: CXIdxIncludedFileInfo): number {
  return addon.get_CXIdxIncludedFileInfo_field(struct, 'isImport');
}

/**
 * Get isAngled field from CXIdxIncludedFileInfo
 */
export function getCXIdxIncludedFileInfo_isAngled(struct: CXIdxIncludedFileInfo): number {
  return addon.get_CXIdxIncludedFileInfo_field(struct, 'isAngled');
}

/**
 * Get isModuleImport field from CXIdxIncludedFileInfo
 */
export function getCXIdxIncludedFileInfo_isModuleImport(struct: CXIdxIncludedFileInfo): number {
  return addon.get_CXIdxIncludedFileInfo_field(struct, 'isModuleImport');
}

/**
 * Get all fields from CXIdxIncludedFileInfo as an object
 */
export function getCXIdxIncludedFileInfoFields(struct: CXIdxIncludedFileInfo): CXIdxIncludedFileInfoInit {
  return {
    hashLoc: addon.get_CXIdxIncludedFileInfo_field(struct, 'hashLoc'),
    filename: addon.get_CXIdxIncludedFileInfo_field(struct, 'filename'),
    file: addon.get_CXIdxIncludedFileInfo_field(struct, 'file'),
    isImport: addon.get_CXIdxIncludedFileInfo_field(struct, 'isImport'),
    isAngled: addon.get_CXIdxIncludedFileInfo_field(struct, 'isAngled'),
    isModuleImport: addon.get_CXIdxIncludedFileInfo_field(struct, 'isModuleImport'),
  };
}

/**
 * Create a new CXIdxImportedASTFileInfo instance
 * Data for IndexerCallbacks#importedASTFile.
 */
export function createCXIdxImportedASTFileInfo(init?: CXIdxImportedASTFileInfoInit): CXIdxImportedASTFileInfo {
  return addon.create_CXIdxImportedASTFileInfo(init);
}

/**
 * Get file field from CXIdxImportedASTFileInfo
 */
export function getCXIdxImportedASTFileInfo_file(struct: CXIdxImportedASTFileInfo): { _type: 'CXFile' } {
  return addon.get_CXIdxImportedASTFileInfo_field(struct, 'file');
}

/**
 * Get module field from CXIdxImportedASTFileInfo
 */
export function getCXIdxImportedASTFileInfo_module(struct: CXIdxImportedASTFileInfo): { _type: 'CXModule' } {
  return addon.get_CXIdxImportedASTFileInfo_field(struct, 'module');
}

/**
 * Get loc field from CXIdxImportedASTFileInfo
 */
export function getCXIdxImportedASTFileInfo_loc(struct: CXIdxImportedASTFileInfo): { _type: 'CXIdxLoc' } {
  return addon.get_CXIdxImportedASTFileInfo_field(struct, 'loc');
}

/**
 * Get isImplicit field from CXIdxImportedASTFileInfo
 */
export function getCXIdxImportedASTFileInfo_isImplicit(struct: CXIdxImportedASTFileInfo): number {
  return addon.get_CXIdxImportedASTFileInfo_field(struct, 'isImplicit');
}

/**
 * Get all fields from CXIdxImportedASTFileInfo as an object
 */
export function getCXIdxImportedASTFileInfoFields(struct: CXIdxImportedASTFileInfo): CXIdxImportedASTFileInfoInit {
  return {
    file: addon.get_CXIdxImportedASTFileInfo_field(struct, 'file'),
    module: addon.get_CXIdxImportedASTFileInfo_field(struct, 'module'),
    loc: addon.get_CXIdxImportedASTFileInfo_field(struct, 'loc'),
    isImplicit: addon.get_CXIdxImportedASTFileInfo_field(struct, 'isImplicit'),
  };
}

export function createCXIdxAttrInfo(init?: CXIdxAttrInfoInit): CXIdxAttrInfo {
  return addon.create_CXIdxAttrInfo(init);
}

/**
 * Get kind field from CXIdxAttrInfo
 */
export function getCXIdxAttrInfo_kind(struct: CXIdxAttrInfo): { _type: 'CXIdxAttrKind' } {
  return addon.get_CXIdxAttrInfo_field(struct, 'kind');
}

/**
 * Get cursor field from CXIdxAttrInfo
 */
export function getCXIdxAttrInfo_cursor(struct: CXIdxAttrInfo): { _type: 'CXCursor' } {
  return addon.get_CXIdxAttrInfo_field(struct, 'cursor');
}

/**
 * Get loc field from CXIdxAttrInfo
 */
export function getCXIdxAttrInfo_loc(struct: CXIdxAttrInfo): { _type: 'CXIdxLoc' } {
  return addon.get_CXIdxAttrInfo_field(struct, 'loc');
}

/**
 * Get all fields from CXIdxAttrInfo as an object
 */
export function getCXIdxAttrInfoFields(struct: CXIdxAttrInfo): CXIdxAttrInfoInit {
  return {
    kind: addon.get_CXIdxAttrInfo_field(struct, 'kind'),
    cursor: addon.get_CXIdxAttrInfo_field(struct, 'cursor'),
    loc: addon.get_CXIdxAttrInfo_field(struct, 'loc'),
  };
}

export function createCXIdxEntityInfo(init?: CXIdxEntityInfoInit): CXIdxEntityInfo {
  return addon.create_CXIdxEntityInfo(init);
}

/**
 * Get kind field from CXIdxEntityInfo
 */
export function getCXIdxEntityInfo_kind(struct: CXIdxEntityInfo): { _type: 'CXIdxEntityKind' } {
  return addon.get_CXIdxEntityInfo_field(struct, 'kind');
}

/**
 * Get templateKind field from CXIdxEntityInfo
 */
export function getCXIdxEntityInfo_templateKind(struct: CXIdxEntityInfo): { _type: 'CXIdxEntityCXXTemplateKind' } {
  return addon.get_CXIdxEntityInfo_field(struct, 'templateKind');
}

/**
 * Get lang field from CXIdxEntityInfo
 */
export function getCXIdxEntityInfo_lang(struct: CXIdxEntityInfo): { _type: 'CXIdxEntityLanguage' } {
  return addon.get_CXIdxEntityInfo_field(struct, 'lang');
}

/**
 * Get name field from CXIdxEntityInfo
 */
export function getCXIdxEntityInfo_name(struct: CXIdxEntityInfo): string {
  return addon.get_CXIdxEntityInfo_field(struct, 'name');
}

/**
 * Get USR field from CXIdxEntityInfo
 */
export function getCXIdxEntityInfo_USR(struct: CXIdxEntityInfo): string {
  return addon.get_CXIdxEntityInfo_field(struct, 'USR');
}

/**
 * Get cursor field from CXIdxEntityInfo
 */
export function getCXIdxEntityInfo_cursor(struct: CXIdxEntityInfo): { _type: 'CXCursor' } {
  return addon.get_CXIdxEntityInfo_field(struct, 'cursor');
}

/**
 * Get attributes field from CXIdxEntityInfo
 */
export function getCXIdxEntityInfo_attributes(struct: CXIdxEntityInfo): { _ptr: unknown } {
  return addon.get_CXIdxEntityInfo_field(struct, 'attributes');
}

/**
 * Get numAttributes field from CXIdxEntityInfo
 */
export function getCXIdxEntityInfo_numAttributes(struct: CXIdxEntityInfo): number {
  return addon.get_CXIdxEntityInfo_field(struct, 'numAttributes');
}

/**
 * Get all fields from CXIdxEntityInfo as an object
 */
export function getCXIdxEntityInfoFields(struct: CXIdxEntityInfo): CXIdxEntityInfoInit {
  return {
    kind: addon.get_CXIdxEntityInfo_field(struct, 'kind'),
    templateKind: addon.get_CXIdxEntityInfo_field(struct, 'templateKind'),
    lang: addon.get_CXIdxEntityInfo_field(struct, 'lang'),
    name: addon.get_CXIdxEntityInfo_field(struct, 'name'),
    USR: addon.get_CXIdxEntityInfo_field(struct, 'USR'),
    cursor: addon.get_CXIdxEntityInfo_field(struct, 'cursor'),
    attributes: addon.get_CXIdxEntityInfo_field(struct, 'attributes'),
    numAttributes: addon.get_CXIdxEntityInfo_field(struct, 'numAttributes'),
  };
}

export function createCXIdxContainerInfo(init?: CXIdxContainerInfoInit): CXIdxContainerInfo {
  return addon.create_CXIdxContainerInfo(init);
}

/**
 * Get cursor field from CXIdxContainerInfo
 */
export function getCXIdxContainerInfo_cursor(struct: CXIdxContainerInfo): { _type: 'CXCursor' } {
  return addon.get_CXIdxContainerInfo_field(struct, 'cursor');
}

/**
 * Get all fields from CXIdxContainerInfo as an object
 */
export function getCXIdxContainerInfoFields(struct: CXIdxContainerInfo): CXIdxContainerInfoInit {
  return {
    cursor: addon.get_CXIdxContainerInfo_field(struct, 'cursor'),
  };
}

export function createCXIdxIBOutletCollectionAttrInfo(init?: CXIdxIBOutletCollectionAttrInfoInit): CXIdxIBOutletCollectionAttrInfo {
  return addon.create_CXIdxIBOutletCollectionAttrInfo(init);
}

/**
 * Get attrInfo field from CXIdxIBOutletCollectionAttrInfo
 */
export function getCXIdxIBOutletCollectionAttrInfo_attrInfo(struct: CXIdxIBOutletCollectionAttrInfo): { _ptr: unknown } {
  return addon.get_CXIdxIBOutletCollectionAttrInfo_field(struct, 'attrInfo');
}

/**
 * Get objcClass field from CXIdxIBOutletCollectionAttrInfo
 */
export function getCXIdxIBOutletCollectionAttrInfo_objcClass(struct: CXIdxIBOutletCollectionAttrInfo): { _ptr: unknown } {
  return addon.get_CXIdxIBOutletCollectionAttrInfo_field(struct, 'objcClass');
}

/**
 * Get classCursor field from CXIdxIBOutletCollectionAttrInfo
 */
export function getCXIdxIBOutletCollectionAttrInfo_classCursor(struct: CXIdxIBOutletCollectionAttrInfo): { _type: 'CXCursor' } {
  return addon.get_CXIdxIBOutletCollectionAttrInfo_field(struct, 'classCursor');
}

/**
 * Get classLoc field from CXIdxIBOutletCollectionAttrInfo
 */
export function getCXIdxIBOutletCollectionAttrInfo_classLoc(struct: CXIdxIBOutletCollectionAttrInfo): { _type: 'CXIdxLoc' } {
  return addon.get_CXIdxIBOutletCollectionAttrInfo_field(struct, 'classLoc');
}

/**
 * Get all fields from CXIdxIBOutletCollectionAttrInfo as an object
 */
export function getCXIdxIBOutletCollectionAttrInfoFields(struct: CXIdxIBOutletCollectionAttrInfo): CXIdxIBOutletCollectionAttrInfoInit {
  return {
    attrInfo: addon.get_CXIdxIBOutletCollectionAttrInfo_field(struct, 'attrInfo'),
    objcClass: addon.get_CXIdxIBOutletCollectionAttrInfo_field(struct, 'objcClass'),
    classCursor: addon.get_CXIdxIBOutletCollectionAttrInfo_field(struct, 'classCursor'),
    classLoc: addon.get_CXIdxIBOutletCollectionAttrInfo_field(struct, 'classLoc'),
  };
}

export function createCXIdxDeclInfo(init?: CXIdxDeclInfoInit): CXIdxDeclInfo {
  return addon.create_CXIdxDeclInfo(init);
}

/**
 * Get entityInfo field from CXIdxDeclInfo
 */
export function getCXIdxDeclInfo_entityInfo(struct: CXIdxDeclInfo): { _ptr: unknown } {
  return addon.get_CXIdxDeclInfo_field(struct, 'entityInfo');
}

/**
 * Get cursor field from CXIdxDeclInfo
 */
export function getCXIdxDeclInfo_cursor(struct: CXIdxDeclInfo): { _type: 'CXCursor' } {
  return addon.get_CXIdxDeclInfo_field(struct, 'cursor');
}

/**
 * Get loc field from CXIdxDeclInfo
 */
export function getCXIdxDeclInfo_loc(struct: CXIdxDeclInfo): { _type: 'CXIdxLoc' } {
  return addon.get_CXIdxDeclInfo_field(struct, 'loc');
}

/**
 * Get semanticContainer field from CXIdxDeclInfo
 */
export function getCXIdxDeclInfo_semanticContainer(struct: CXIdxDeclInfo): { _ptr: unknown } {
  return addon.get_CXIdxDeclInfo_field(struct, 'semanticContainer');
}

/**
 * Get lexicalContainer field from CXIdxDeclInfo
 */
export function getCXIdxDeclInfo_lexicalContainer(struct: CXIdxDeclInfo): { _ptr: unknown } {
  return addon.get_CXIdxDeclInfo_field(struct, 'lexicalContainer');
}

/**
 * Get isRedeclaration field from CXIdxDeclInfo
 */
export function getCXIdxDeclInfo_isRedeclaration(struct: CXIdxDeclInfo): number {
  return addon.get_CXIdxDeclInfo_field(struct, 'isRedeclaration');
}

/**
 * Get isDefinition field from CXIdxDeclInfo
 */
export function getCXIdxDeclInfo_isDefinition(struct: CXIdxDeclInfo): number {
  return addon.get_CXIdxDeclInfo_field(struct, 'isDefinition');
}

/**
 * Get isContainer field from CXIdxDeclInfo
 */
export function getCXIdxDeclInfo_isContainer(struct: CXIdxDeclInfo): number {
  return addon.get_CXIdxDeclInfo_field(struct, 'isContainer');
}

/**
 * Get declAsContainer field from CXIdxDeclInfo
 */
export function getCXIdxDeclInfo_declAsContainer(struct: CXIdxDeclInfo): { _ptr: unknown } {
  return addon.get_CXIdxDeclInfo_field(struct, 'declAsContainer');
}

/**
 * Get isImplicit field from CXIdxDeclInfo
 */
export function getCXIdxDeclInfo_isImplicit(struct: CXIdxDeclInfo): number {
  return addon.get_CXIdxDeclInfo_field(struct, 'isImplicit');
}

/**
 * Get attributes field from CXIdxDeclInfo
 */
export function getCXIdxDeclInfo_attributes(struct: CXIdxDeclInfo): { _ptr: unknown } {
  return addon.get_CXIdxDeclInfo_field(struct, 'attributes');
}

/**
 * Get numAttributes field from CXIdxDeclInfo
 */
export function getCXIdxDeclInfo_numAttributes(struct: CXIdxDeclInfo): number {
  return addon.get_CXIdxDeclInfo_field(struct, 'numAttributes');
}

/**
 * Get flags field from CXIdxDeclInfo
 */
export function getCXIdxDeclInfo_flags(struct: CXIdxDeclInfo): number {
  return addon.get_CXIdxDeclInfo_field(struct, 'flags');
}

/**
 * Get all fields from CXIdxDeclInfo as an object
 */
export function getCXIdxDeclInfoFields(struct: CXIdxDeclInfo): CXIdxDeclInfoInit {
  return {
    entityInfo: addon.get_CXIdxDeclInfo_field(struct, 'entityInfo'),
    cursor: addon.get_CXIdxDeclInfo_field(struct, 'cursor'),
    loc: addon.get_CXIdxDeclInfo_field(struct, 'loc'),
    semanticContainer: addon.get_CXIdxDeclInfo_field(struct, 'semanticContainer'),
    lexicalContainer: addon.get_CXIdxDeclInfo_field(struct, 'lexicalContainer'),
    isRedeclaration: addon.get_CXIdxDeclInfo_field(struct, 'isRedeclaration'),
    isDefinition: addon.get_CXIdxDeclInfo_field(struct, 'isDefinition'),
    isContainer: addon.get_CXIdxDeclInfo_field(struct, 'isContainer'),
    declAsContainer: addon.get_CXIdxDeclInfo_field(struct, 'declAsContainer'),
    isImplicit: addon.get_CXIdxDeclInfo_field(struct, 'isImplicit'),
    attributes: addon.get_CXIdxDeclInfo_field(struct, 'attributes'),
    numAttributes: addon.get_CXIdxDeclInfo_field(struct, 'numAttributes'),
    flags: addon.get_CXIdxDeclInfo_field(struct, 'flags'),
  };
}

export function createCXIdxObjCContainerDeclInfo(init?: CXIdxObjCContainerDeclInfoInit): CXIdxObjCContainerDeclInfo {
  return addon.create_CXIdxObjCContainerDeclInfo(init);
}

/**
 * Get declInfo field from CXIdxObjCContainerDeclInfo
 */
export function getCXIdxObjCContainerDeclInfo_declInfo(struct: CXIdxObjCContainerDeclInfo): { _ptr: unknown } {
  return addon.get_CXIdxObjCContainerDeclInfo_field(struct, 'declInfo');
}

/**
 * Get kind field from CXIdxObjCContainerDeclInfo
 */
export function getCXIdxObjCContainerDeclInfo_kind(struct: CXIdxObjCContainerDeclInfo): { _type: 'CXIdxObjCContainerKind' } {
  return addon.get_CXIdxObjCContainerDeclInfo_field(struct, 'kind');
}

/**
 * Get all fields from CXIdxObjCContainerDeclInfo as an object
 */
export function getCXIdxObjCContainerDeclInfoFields(struct: CXIdxObjCContainerDeclInfo): CXIdxObjCContainerDeclInfoInit {
  return {
    declInfo: addon.get_CXIdxObjCContainerDeclInfo_field(struct, 'declInfo'),
    kind: addon.get_CXIdxObjCContainerDeclInfo_field(struct, 'kind'),
  };
}

export function createCXIdxBaseClassInfo(init?: CXIdxBaseClassInfoInit): CXIdxBaseClassInfo {
  return addon.create_CXIdxBaseClassInfo(init);
}

/**
 * Get base field from CXIdxBaseClassInfo
 */
export function getCXIdxBaseClassInfo_base(struct: CXIdxBaseClassInfo): { _ptr: unknown } {
  return addon.get_CXIdxBaseClassInfo_field(struct, 'base');
}

/**
 * Get cursor field from CXIdxBaseClassInfo
 */
export function getCXIdxBaseClassInfo_cursor(struct: CXIdxBaseClassInfo): { _type: 'CXCursor' } {
  return addon.get_CXIdxBaseClassInfo_field(struct, 'cursor');
}

/**
 * Get loc field from CXIdxBaseClassInfo
 */
export function getCXIdxBaseClassInfo_loc(struct: CXIdxBaseClassInfo): { _type: 'CXIdxLoc' } {
  return addon.get_CXIdxBaseClassInfo_field(struct, 'loc');
}

/**
 * Get all fields from CXIdxBaseClassInfo as an object
 */
export function getCXIdxBaseClassInfoFields(struct: CXIdxBaseClassInfo): CXIdxBaseClassInfoInit {
  return {
    base: addon.get_CXIdxBaseClassInfo_field(struct, 'base'),
    cursor: addon.get_CXIdxBaseClassInfo_field(struct, 'cursor'),
    loc: addon.get_CXIdxBaseClassInfo_field(struct, 'loc'),
  };
}

export function createCXIdxObjCProtocolRefInfo(init?: CXIdxObjCProtocolRefInfoInit): CXIdxObjCProtocolRefInfo {
  return addon.create_CXIdxObjCProtocolRefInfo(init);
}

/**
 * Get protocol field from CXIdxObjCProtocolRefInfo
 */
export function getCXIdxObjCProtocolRefInfo_protocol(struct: CXIdxObjCProtocolRefInfo): { _ptr: unknown } {
  return addon.get_CXIdxObjCProtocolRefInfo_field(struct, 'protocol');
}

/**
 * Get cursor field from CXIdxObjCProtocolRefInfo
 */
export function getCXIdxObjCProtocolRefInfo_cursor(struct: CXIdxObjCProtocolRefInfo): { _type: 'CXCursor' } {
  return addon.get_CXIdxObjCProtocolRefInfo_field(struct, 'cursor');
}

/**
 * Get loc field from CXIdxObjCProtocolRefInfo
 */
export function getCXIdxObjCProtocolRefInfo_loc(struct: CXIdxObjCProtocolRefInfo): { _type: 'CXIdxLoc' } {
  return addon.get_CXIdxObjCProtocolRefInfo_field(struct, 'loc');
}

/**
 * Get all fields from CXIdxObjCProtocolRefInfo as an object
 */
export function getCXIdxObjCProtocolRefInfoFields(struct: CXIdxObjCProtocolRefInfo): CXIdxObjCProtocolRefInfoInit {
  return {
    protocol: addon.get_CXIdxObjCProtocolRefInfo_field(struct, 'protocol'),
    cursor: addon.get_CXIdxObjCProtocolRefInfo_field(struct, 'cursor'),
    loc: addon.get_CXIdxObjCProtocolRefInfo_field(struct, 'loc'),
  };
}

export function createCXIdxObjCProtocolRefListInfo(init?: CXIdxObjCProtocolRefListInfoInit): CXIdxObjCProtocolRefListInfo {
  return addon.create_CXIdxObjCProtocolRefListInfo(init);
}

/**
 * Get protocols field from CXIdxObjCProtocolRefListInfo
 */
export function getCXIdxObjCProtocolRefListInfo_protocols(struct: CXIdxObjCProtocolRefListInfo): { _ptr: unknown } {
  return addon.get_CXIdxObjCProtocolRefListInfo_field(struct, 'protocols');
}

/**
 * Get numProtocols field from CXIdxObjCProtocolRefListInfo
 */
export function getCXIdxObjCProtocolRefListInfo_numProtocols(struct: CXIdxObjCProtocolRefListInfo): number {
  return addon.get_CXIdxObjCProtocolRefListInfo_field(struct, 'numProtocols');
}

/**
 * Get all fields from CXIdxObjCProtocolRefListInfo as an object
 */
export function getCXIdxObjCProtocolRefListInfoFields(struct: CXIdxObjCProtocolRefListInfo): CXIdxObjCProtocolRefListInfoInit {
  return {
    protocols: addon.get_CXIdxObjCProtocolRefListInfo_field(struct, 'protocols'),
    numProtocols: addon.get_CXIdxObjCProtocolRefListInfo_field(struct, 'numProtocols'),
  };
}

export function createCXIdxObjCInterfaceDeclInfo(init?: CXIdxObjCInterfaceDeclInfoInit): CXIdxObjCInterfaceDeclInfo {
  return addon.create_CXIdxObjCInterfaceDeclInfo(init);
}

/**
 * Get containerInfo field from CXIdxObjCInterfaceDeclInfo
 */
export function getCXIdxObjCInterfaceDeclInfo_containerInfo(struct: CXIdxObjCInterfaceDeclInfo): { _ptr: unknown } {
  return addon.get_CXIdxObjCInterfaceDeclInfo_field(struct, 'containerInfo');
}

/**
 * Get superInfo field from CXIdxObjCInterfaceDeclInfo
 */
export function getCXIdxObjCInterfaceDeclInfo_superInfo(struct: CXIdxObjCInterfaceDeclInfo): { _ptr: unknown } {
  return addon.get_CXIdxObjCInterfaceDeclInfo_field(struct, 'superInfo');
}

/**
 * Get protocols field from CXIdxObjCInterfaceDeclInfo
 */
export function getCXIdxObjCInterfaceDeclInfo_protocols(struct: CXIdxObjCInterfaceDeclInfo): { _ptr: unknown } {
  return addon.get_CXIdxObjCInterfaceDeclInfo_field(struct, 'protocols');
}

/**
 * Get all fields from CXIdxObjCInterfaceDeclInfo as an object
 */
export function getCXIdxObjCInterfaceDeclInfoFields(struct: CXIdxObjCInterfaceDeclInfo): CXIdxObjCInterfaceDeclInfoInit {
  return {
    containerInfo: addon.get_CXIdxObjCInterfaceDeclInfo_field(struct, 'containerInfo'),
    superInfo: addon.get_CXIdxObjCInterfaceDeclInfo_field(struct, 'superInfo'),
    protocols: addon.get_CXIdxObjCInterfaceDeclInfo_field(struct, 'protocols'),
  };
}

export function createCXIdxObjCCategoryDeclInfo(init?: CXIdxObjCCategoryDeclInfoInit): CXIdxObjCCategoryDeclInfo {
  return addon.create_CXIdxObjCCategoryDeclInfo(init);
}

/**
 * Get containerInfo field from CXIdxObjCCategoryDeclInfo
 */
export function getCXIdxObjCCategoryDeclInfo_containerInfo(struct: CXIdxObjCCategoryDeclInfo): { _ptr: unknown } {
  return addon.get_CXIdxObjCCategoryDeclInfo_field(struct, 'containerInfo');
}

/**
 * Get objcClass field from CXIdxObjCCategoryDeclInfo
 */
export function getCXIdxObjCCategoryDeclInfo_objcClass(struct: CXIdxObjCCategoryDeclInfo): { _ptr: unknown } {
  return addon.get_CXIdxObjCCategoryDeclInfo_field(struct, 'objcClass');
}

/**
 * Get classCursor field from CXIdxObjCCategoryDeclInfo
 */
export function getCXIdxObjCCategoryDeclInfo_classCursor(struct: CXIdxObjCCategoryDeclInfo): { _type: 'CXCursor' } {
  return addon.get_CXIdxObjCCategoryDeclInfo_field(struct, 'classCursor');
}

/**
 * Get classLoc field from CXIdxObjCCategoryDeclInfo
 */
export function getCXIdxObjCCategoryDeclInfo_classLoc(struct: CXIdxObjCCategoryDeclInfo): { _type: 'CXIdxLoc' } {
  return addon.get_CXIdxObjCCategoryDeclInfo_field(struct, 'classLoc');
}

/**
 * Get protocols field from CXIdxObjCCategoryDeclInfo
 */
export function getCXIdxObjCCategoryDeclInfo_protocols(struct: CXIdxObjCCategoryDeclInfo): { _ptr: unknown } {
  return addon.get_CXIdxObjCCategoryDeclInfo_field(struct, 'protocols');
}

/**
 * Get all fields from CXIdxObjCCategoryDeclInfo as an object
 */
export function getCXIdxObjCCategoryDeclInfoFields(struct: CXIdxObjCCategoryDeclInfo): CXIdxObjCCategoryDeclInfoInit {
  return {
    containerInfo: addon.get_CXIdxObjCCategoryDeclInfo_field(struct, 'containerInfo'),
    objcClass: addon.get_CXIdxObjCCategoryDeclInfo_field(struct, 'objcClass'),
    classCursor: addon.get_CXIdxObjCCategoryDeclInfo_field(struct, 'classCursor'),
    classLoc: addon.get_CXIdxObjCCategoryDeclInfo_field(struct, 'classLoc'),
    protocols: addon.get_CXIdxObjCCategoryDeclInfo_field(struct, 'protocols'),
  };
}

export function createCXIdxObjCPropertyDeclInfo(init?: CXIdxObjCPropertyDeclInfoInit): CXIdxObjCPropertyDeclInfo {
  return addon.create_CXIdxObjCPropertyDeclInfo(init);
}

/**
 * Get declInfo field from CXIdxObjCPropertyDeclInfo
 */
export function getCXIdxObjCPropertyDeclInfo_declInfo(struct: CXIdxObjCPropertyDeclInfo): { _ptr: unknown } {
  return addon.get_CXIdxObjCPropertyDeclInfo_field(struct, 'declInfo');
}

/**
 * Get getter field from CXIdxObjCPropertyDeclInfo
 */
export function getCXIdxObjCPropertyDeclInfo_getter(struct: CXIdxObjCPropertyDeclInfo): { _ptr: unknown } {
  return addon.get_CXIdxObjCPropertyDeclInfo_field(struct, 'getter');
}

/**
 * Get setter field from CXIdxObjCPropertyDeclInfo
 */
export function getCXIdxObjCPropertyDeclInfo_setter(struct: CXIdxObjCPropertyDeclInfo): { _ptr: unknown } {
  return addon.get_CXIdxObjCPropertyDeclInfo_field(struct, 'setter');
}

/**
 * Get all fields from CXIdxObjCPropertyDeclInfo as an object
 */
export function getCXIdxObjCPropertyDeclInfoFields(struct: CXIdxObjCPropertyDeclInfo): CXIdxObjCPropertyDeclInfoInit {
  return {
    declInfo: addon.get_CXIdxObjCPropertyDeclInfo_field(struct, 'declInfo'),
    getter: addon.get_CXIdxObjCPropertyDeclInfo_field(struct, 'getter'),
    setter: addon.get_CXIdxObjCPropertyDeclInfo_field(struct, 'setter'),
  };
}

export function createCXIdxCXXClassDeclInfo(init?: CXIdxCXXClassDeclInfoInit): CXIdxCXXClassDeclInfo {
  return addon.create_CXIdxCXXClassDeclInfo(init);
}

/**
 * Get declInfo field from CXIdxCXXClassDeclInfo
 */
export function getCXIdxCXXClassDeclInfo_declInfo(struct: CXIdxCXXClassDeclInfo): { _ptr: unknown } {
  return addon.get_CXIdxCXXClassDeclInfo_field(struct, 'declInfo');
}

/**
 * Get bases field from CXIdxCXXClassDeclInfo
 */
export function getCXIdxCXXClassDeclInfo_bases(struct: CXIdxCXXClassDeclInfo): { _ptr: unknown } {
  return addon.get_CXIdxCXXClassDeclInfo_field(struct, 'bases');
}

/**
 * Get numBases field from CXIdxCXXClassDeclInfo
 */
export function getCXIdxCXXClassDeclInfo_numBases(struct: CXIdxCXXClassDeclInfo): number {
  return addon.get_CXIdxCXXClassDeclInfo_field(struct, 'numBases');
}

/**
 * Get all fields from CXIdxCXXClassDeclInfo as an object
 */
export function getCXIdxCXXClassDeclInfoFields(struct: CXIdxCXXClassDeclInfo): CXIdxCXXClassDeclInfoInit {
  return {
    declInfo: addon.get_CXIdxCXXClassDeclInfo_field(struct, 'declInfo'),
    bases: addon.get_CXIdxCXXClassDeclInfo_field(struct, 'bases'),
    numBases: addon.get_CXIdxCXXClassDeclInfo_field(struct, 'numBases'),
  };
}

/**
 * Create a new CXIdxEntityRefInfo instance
 * Data for IndexerCallbacks#indexEntityReference.
 */
export function createCXIdxEntityRefInfo(init?: CXIdxEntityRefInfoInit): CXIdxEntityRefInfo {
  return addon.create_CXIdxEntityRefInfo(init);
}

/**
 * Get kind field from CXIdxEntityRefInfo
 */
export function getCXIdxEntityRefInfo_kind(struct: CXIdxEntityRefInfo): { _type: 'CXIdxEntityRefKind' } {
  return addon.get_CXIdxEntityRefInfo_field(struct, 'kind');
}

/**
 * Get cursor field from CXIdxEntityRefInfo
 */
export function getCXIdxEntityRefInfo_cursor(struct: CXIdxEntityRefInfo): { _type: 'CXCursor' } {
  return addon.get_CXIdxEntityRefInfo_field(struct, 'cursor');
}

/**
 * Get loc field from CXIdxEntityRefInfo
 */
export function getCXIdxEntityRefInfo_loc(struct: CXIdxEntityRefInfo): { _type: 'CXIdxLoc' } {
  return addon.get_CXIdxEntityRefInfo_field(struct, 'loc');
}

/**
 * Get referencedEntity field from CXIdxEntityRefInfo
 */
export function getCXIdxEntityRefInfo_referencedEntity(struct: CXIdxEntityRefInfo): { _ptr: unknown } {
  return addon.get_CXIdxEntityRefInfo_field(struct, 'referencedEntity');
}

/**
 * Get parentEntity field from CXIdxEntityRefInfo
 */
export function getCXIdxEntityRefInfo_parentEntity(struct: CXIdxEntityRefInfo): { _ptr: unknown } {
  return addon.get_CXIdxEntityRefInfo_field(struct, 'parentEntity');
}

/**
 * Get container field from CXIdxEntityRefInfo
 */
export function getCXIdxEntityRefInfo_container(struct: CXIdxEntityRefInfo): { _ptr: unknown } {
  return addon.get_CXIdxEntityRefInfo_field(struct, 'container');
}

/**
 * Get role field from CXIdxEntityRefInfo
 */
export function getCXIdxEntityRefInfo_role(struct: CXIdxEntityRefInfo): { _type: 'CXSymbolRole' } {
  return addon.get_CXIdxEntityRefInfo_field(struct, 'role');
}

/**
 * Get all fields from CXIdxEntityRefInfo as an object
 */
export function getCXIdxEntityRefInfoFields(struct: CXIdxEntityRefInfo): CXIdxEntityRefInfoInit {
  return {
    kind: addon.get_CXIdxEntityRefInfo_field(struct, 'kind'),
    cursor: addon.get_CXIdxEntityRefInfo_field(struct, 'cursor'),
    loc: addon.get_CXIdxEntityRefInfo_field(struct, 'loc'),
    referencedEntity: addon.get_CXIdxEntityRefInfo_field(struct, 'referencedEntity'),
    parentEntity: addon.get_CXIdxEntityRefInfo_field(struct, 'parentEntity'),
    container: addon.get_CXIdxEntityRefInfo_field(struct, 'container'),
    role: addon.get_CXIdxEntityRefInfo_field(struct, 'role'),
  };
}

/**
 * Create a new IndexerCallbacks instance
 * A group of callbacks used by #clang_indexSourceFile and
#clang_indexTranslationUnit.
 */
export function createIndexerCallbacks(init?: IndexerCallbacksInit): IndexerCallbacks {
  return addon.create_IndexerCallbacks(init);
}

/**
 * Get abortQuery field from IndexerCallbacks
 */
export function getIndexerCallbacks_abortQuery(struct: IndexerCallbacks): { _type: 'int (*)(CXClientData, void *)' } {
  return addon.get_IndexerCallbacks_field(struct, 'abortQuery');
}

/**
 * Get diagnostic field from IndexerCallbacks
 */
export function getIndexerCallbacks_diagnostic(struct: IndexerCallbacks): { _type: 'void (*)(CXClientData, CXDiagnosticSet, void *)' } {
  return addon.get_IndexerCallbacks_field(struct, 'diagnostic');
}

/**
 * Get enteredMainFile field from IndexerCallbacks
 */
export function getIndexerCallbacks_enteredMainFile(struct: IndexerCallbacks): { _type: 'CXIdxClientFile (*)(CXClientData, CXFile, void *)' } {
  return addon.get_IndexerCallbacks_field(struct, 'enteredMainFile');
}

/**
 * Get ppIncludedFile field from IndexerCallbacks
 */
export function getIndexerCallbacks_ppIncludedFile(struct: IndexerCallbacks): { _type: 'CXIdxClientFile (*)(CXClientData, const CXIdxIncludedFileInfo *)' } {
  return addon.get_IndexerCallbacks_field(struct, 'ppIncludedFile');
}

/**
 * Get importedASTFile field from IndexerCallbacks
 */
export function getIndexerCallbacks_importedASTFile(struct: IndexerCallbacks): { _type: 'CXIdxClientASTFile (*)(CXClientData, const CXIdxImportedASTFileInfo *)' } {
  return addon.get_IndexerCallbacks_field(struct, 'importedASTFile');
}

/**
 * Get startedTranslationUnit field from IndexerCallbacks
 */
export function getIndexerCallbacks_startedTranslationUnit(struct: IndexerCallbacks): { _type: 'CXIdxClientContainer (*)(CXClientData, void *)' } {
  return addon.get_IndexerCallbacks_field(struct, 'startedTranslationUnit');
}

/**
 * Get indexDeclaration field from IndexerCallbacks
 */
export function getIndexerCallbacks_indexDeclaration(struct: IndexerCallbacks): { _type: 'void (*)(CXClientData, const CXIdxDeclInfo *)' } {
  return addon.get_IndexerCallbacks_field(struct, 'indexDeclaration');
}

/**
 * Get indexEntityReference field from IndexerCallbacks
 */
export function getIndexerCallbacks_indexEntityReference(struct: IndexerCallbacks): { _type: 'void (*)(CXClientData, const CXIdxEntityRefInfo *)' } {
  return addon.get_IndexerCallbacks_field(struct, 'indexEntityReference');
}

/**
 * Get all fields from IndexerCallbacks as an object
 */
export function getIndexerCallbacksFields(struct: IndexerCallbacks): IndexerCallbacksInit {
  return {
    abortQuery: addon.get_IndexerCallbacks_field(struct, 'abortQuery'),
    diagnostic: addon.get_IndexerCallbacks_field(struct, 'diagnostic'),
    enteredMainFile: addon.get_IndexerCallbacks_field(struct, 'enteredMainFile'),
    ppIncludedFile: addon.get_IndexerCallbacks_field(struct, 'ppIncludedFile'),
    importedASTFile: addon.get_IndexerCallbacks_field(struct, 'importedASTFile'),
    startedTranslationUnit: addon.get_IndexerCallbacks_field(struct, 'startedTranslationUnit'),
    indexDeclaration: addon.get_IndexerCallbacks_field(struct, 'indexDeclaration'),
    indexEntityReference: addon.get_IndexerCallbacks_field(struct, 'indexEntityReference'),
  };
}

/**
 * Create a new CXString instance
 * A character string.

The \c CXString type is used to return strings from the interface when
the ownership of that string might differ from one call to the next.
Use \c clang_getCString() to retrieve the string data and, once finished
with the string data, call \c clang_disposeString() to free the string.
 */
export function createCXString(init?: CXStringInit): CXString {
  return addon.create_CXString(init);
}

/**
 * Get data field from CXString
 */
export function getCXString_data(struct: CXString): unknown {
  return addon.get_CXString_field(struct, 'data');
}

/**
 * Get private_flags field from CXString
 */
export function getCXString_private_flags(struct: CXString): number {
  return addon.get_CXString_field(struct, 'private_flags');
}

/**
 * Get all fields from CXString as an object
 */
export function getCXStringFields(struct: CXString): CXStringInit {
  return {
    data: addon.get_CXString_field(struct, 'data'),
    private_flags: addon.get_CXString_field(struct, 'private_flags'),
  };
}

export function createCXStringSet(init?: CXStringSetInit): CXStringSet {
  return addon.create_CXStringSet(init);
}

/**
 * Get Strings field from CXStringSet
 */
export function getCXStringSet_Strings(struct: CXStringSet): { _ptr: unknown } {
  return addon.get_CXStringSet_field(struct, 'Strings');
}

/**
 * Get Count field from CXStringSet
 */
export function getCXStringSet_Count(struct: CXStringSet): number {
  return addon.get_CXStringSet_field(struct, 'Count');
}

/**
 * Get all fields from CXStringSet as an object
 */
export function getCXStringSetFields(struct: CXStringSet): CXStringSetInit {
  return {
    Strings: addon.get_CXStringSet_field(struct, 'Strings'),
    Count: addon.get_CXStringSet_field(struct, 'Count'),
  };
}

/**
 * Create a new CXSourceLocation instance
 * Identifies a specific source location within a translation
unit.

Use clang_getExpansionLocation() or clang_getSpellingLocation()
to map a source location to a particular file, line, and column.
 */
export function createCXSourceLocation(init?: CXSourceLocationInit): CXSourceLocation {
  return addon.create_CXSourceLocation(init);
}

/**
 * Get ptr_data field from CXSourceLocation
 */
export function getCXSourceLocation_ptr_data(struct: CXSourceLocation): unknown[] {
  return addon.get_CXSourceLocation_field(struct, 'ptr_data');
}

/**
 * Get int_data field from CXSourceLocation
 */
export function getCXSourceLocation_int_data(struct: CXSourceLocation): number {
  return addon.get_CXSourceLocation_field(struct, 'int_data');
}

/**
 * Get all fields from CXSourceLocation as an object
 */
export function getCXSourceLocationFields(struct: CXSourceLocation): CXSourceLocationInit {
  return {
    ptr_data: addon.get_CXSourceLocation_field(struct, 'ptr_data'),
    int_data: addon.get_CXSourceLocation_field(struct, 'int_data'),
  };
}

/**
 * Create a new CXSourceRange instance
 * Identifies a half-open character range in the source code.

Use clang_getRangeStart() and clang_getRangeEnd() to retrieve the
starting and end locations from a source range, respectively.
 */
export function createCXSourceRange(init?: CXSourceRangeInit): CXSourceRange {
  return addon.create_CXSourceRange(init);
}

/**
 * Get ptr_data field from CXSourceRange
 */
export function getCXSourceRange_ptr_data(struct: CXSourceRange): unknown[] {
  return addon.get_CXSourceRange_field(struct, 'ptr_data');
}

/**
 * Get begin_int_data field from CXSourceRange
 */
export function getCXSourceRange_begin_int_data(struct: CXSourceRange): number {
  return addon.get_CXSourceRange_field(struct, 'begin_int_data');
}

/**
 * Get end_int_data field from CXSourceRange
 */
export function getCXSourceRange_end_int_data(struct: CXSourceRange): number {
  return addon.get_CXSourceRange_field(struct, 'end_int_data');
}

/**
 * Get all fields from CXSourceRange as an object
 */
export function getCXSourceRangeFields(struct: CXSourceRange): CXSourceRangeInit {
  return {
    ptr_data: addon.get_CXSourceRange_field(struct, 'ptr_data'),
    begin_int_data: addon.get_CXSourceRange_field(struct, 'begin_int_data'),
    end_int_data: addon.get_CXSourceRange_field(struct, 'end_int_data'),
  };
}

/**
 * Create a new CXSourceRangeList instance
 * Identifies an array of ranges.
 */
export function createCXSourceRangeList(init?: CXSourceRangeListInit): CXSourceRangeList {
  return addon.create_CXSourceRangeList(init);
}

/**
 * Get count field from CXSourceRangeList
 */
export function getCXSourceRangeList_count(struct: CXSourceRangeList): number {
  return addon.get_CXSourceRangeList_field(struct, 'count');
}

/**
 * Get ranges field from CXSourceRangeList
 */
export function getCXSourceRangeList_ranges(struct: CXSourceRangeList): { _ptr: unknown } {
  return addon.get_CXSourceRangeList_field(struct, 'ranges');
}

/**
 * Get all fields from CXSourceRangeList as an object
 */
export function getCXSourceRangeListFields(struct: CXSourceRangeList): CXSourceRangeListInit {
  return {
    count: addon.get_CXSourceRangeList_field(struct, 'count'),
    ranges: addon.get_CXSourceRangeList_field(struct, 'ranges'),
  };
}
