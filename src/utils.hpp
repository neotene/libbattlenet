#pragma once

namespace bnetcpp {

template <class F>
class on_scope_exit {
   private:
    F func_;

   public:
    on_scope_exit(F func)
        : func_(func) {
    }

    ~on_scope_exit() {
        func_();
    }
};

}   // namespace bnetcpp
