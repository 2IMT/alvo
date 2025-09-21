#include "diag.h"

namespace alvo::diag {

    DiagEmitter::DiagEmitter(DiagHandler handler) :
        m_handler(std::move(handler)) { }

    void DiagEmitter::emit(Diag diag) { m_handler(diag); }

}
