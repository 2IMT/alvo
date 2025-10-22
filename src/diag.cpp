#include "diag.h"

namespace alvo::diag {

    Warn::Warn() :
        val(None {}) { }

    Warn::Warn(const Val& val) :
        val(val) { }

    Err::Err() :
        val(None {}) { }

    Err::Err(const Val& val) :
        val(val) { };

    DiagEmitter::DiagEmitter(DiagHandler handler) :
        m_handler(std::move(handler)) { }

    void DiagEmitter::emit(Diag diag) { m_handler(diag); }

}
