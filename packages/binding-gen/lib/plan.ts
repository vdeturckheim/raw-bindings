// Strategy plan types for high-level API generation

export interface ResourcePlan {
  name: string;            // TS class name
  ctype?: string;          // underlying C typedef name (opaque handle)
  create?: string;         // C function to construct
  destroy?: string;        // C function to free
  methods?: Array<{
    c: string;             // underlying C function
    name?: string;         // JS method name (defaults derived)
    params?: Array<{ name: string; type?: string }>; // override names if needed
    returns?: string;
  }>;
  views?: Array<{
    name: string;          // method name like toBuffer
    ptr: string;           // C function returning pointer
    length?: string;       // C function/field returning length
    copy?: boolean;        // copy memory (default true)
  }>;
}

export interface EnumPlan { name: string; exposeAs?: 'const' | 'type' }

export interface CallbackPlan {
  name: string;
  ctype: string;           // function pointer signature
  lifetime?: string;       // resource:Name to tie lifetime
}

export interface ErrorRule { function: string; rule: 'nonZeroIsError' | 'negativeIsError'; throw?: boolean }

export interface StrategyPlan {
  resources?: ResourcePlan[];
  enums?: EnumPlan[];
  callbacks?: CallbackPlan[];
  errors?: ErrorRule[];
  ownership?: Array<{ owner: string; child: string }>;
}

