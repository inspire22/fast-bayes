#include <ruby.h>
#include "bayes.h"

static void fb_free(void* ptr) {
    delete static_cast<Bayes*>(ptr);
}

static const rb_data_type_t fb_type = {
    "FastBayes",
    {
        nullptr,
        fb_free,
        nullptr,
    },
    nullptr,
    nullptr,
    RUBY_TYPED_FREE_IMMEDIATELY
};

static VALUE fb_alloc(VALUE klass) {
    Bayes* b = new Bayes();
    return TypedData_Wrap_Struct(klass, &fb_type, b);
}

static VALUE fb_initialize(VALUE self) {
    return self;
}

static VALUE fb_observe(VALUE self, VALUE label, VALUE data) {
    Bayes* b;
    TypedData_Get_Struct(self, Bayes, &fb_type, b);
    StringValue(label);
    StringValue(data);
    std::string label_str(RSTRING_PTR(label), RSTRING_LEN(label));
    std::string data_str(RSTRING_PTR(data), RSTRING_LEN(data));
    b->observe(label_str, data_str);
    return Qnil;
}

static VALUE fb_classify(VALUE self, VALUE data) {
    Bayes* b;
    TypedData_Get_Struct(self, Bayes, &fb_type, b);
    StringValue(data);
    std::string data_str(RSTRING_PTR(data), RSTRING_LEN(data));
    std::string res = b->classify(data_str);
    return rb_str_new(res.data(), res.size());
}

extern "C" void Init_fast_bayes() {
    VALUE rb_cFastBayes = rb_define_class("FastBayes", rb_cObject);
    rb_define_alloc_func(rb_cFastBayes, fb_alloc);
    rb_define_method(rb_cFastBayes, "initialize", RUBY_METHOD_FUNC(fb_initialize), 0);
    rb_define_method(rb_cFastBayes, "observe", RUBY_METHOD_FUNC(fb_observe), 2);
    rb_define_method(rb_cFastBayes, "classify", RUBY_METHOD_FUNC(fb_classify), 1);
}
