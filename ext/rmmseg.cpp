#include <ruby.h>

#include "token.h"
#include "dict.h"
#include "complex.h"

using namespace std;

extern "C" {

    /*****************************************
     * 
     * Normal interface
     *
     *****************************************/

    /*********************
     * RMMSeg module
     *********************/
    static VALUE mRMMSeg;

    /**********************
     * Token Class
     **********************/
    struct Token
    {
        VALUE text;
        VALUE start;
        VALUE end;
    };

    static void tk_mark(Token *t)
    {
        // start and end are Fixnums, no need to mark them
        rb_gc_mark(t->text);
    }
    static void tk_free(Token *t)
    {
        free(t);
    }

    static VALUE cToken;

    void Init_rmmseg()
    {
        mRMMSeg = rb_define_module("RMMSeg");
        cToken = rb_define_class_under(mRMMSeg, "Token", rb_cObject);
        rb_undef_method(rb_singleton_class(cToken), "new");
    }
}
