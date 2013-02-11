/*
* German M. Bravo (Kronuz) <german.mb@gmail.com>
*
* MIT license (http://www.opensource.org/licenses/mit-license.php)
* Copyright (c) 2011 German M. Bravo (Kronuz), All rights reserved.
*/
#include <Python.h>
#include <structmember.h>
#include <unicodeobject.h>

#include "utils.h"
#include "token.h"
#include "dict.h"
#include "algor.h"


/* Dictionary */

typedef struct {
	PyObject_HEAD
} mmseg_Dictionary;

static PyObject *
mmseg_Dictionary_load_chars(PyObject *self, PyObject *args)
{
	char *path;
	if (PyArg_ParseTuple(args, "s", &path)) {
		if (rmmseg::dict::load_chars(path)) {
			Py_INCREF(Py_True);
			return (PyObject *)Py_True;
		}
	}
	Py_INCREF(Py_False);
	return (PyObject *)Py_False;
}

static PyObject *
mmseg_Dictionary_load_words(PyObject *self, PyObject *args)
{
	char *path;
	if (PyArg_ParseTuple(args, "s", &path)) {
		if (rmmseg::dict::load_words(path)) {
			Py_INCREF(Py_True);
			return (PyObject *)Py_True;
		}
	}
	Py_INCREF(Py_False);
	return (PyObject *)Py_False;
}

static PyObject *
mmseg_Dictionary_add(PyObject *self, PyObject *args, PyObject *kwds)
{
	char *utf8 = NULL;
	PyObject *obj, *tmp = NULL;
	int chars = -1, freq = 0;

	static const char *kwlist[] = {"word", "chars", "freq", NULL};

	if (PyArg_ParseTupleAndKeywords(args, kwds, "O|ii", (char **)(kwlist), &obj, &chars, &freq)) {
		if (PyString_Check(obj)) {
			/* A plain ASCII string is also a valid UTF-8 string */
			utf8 = PyString_AsString(obj);
			if (chars == -1) {
				tmp = PyUnicode_DecodeUTF8(utf8, strlen(utf8), "strict");
				if (tmp == NULL) {
					return NULL;
				}
				chars = static_cast<int>(PyUnicode_GET_SIZE(tmp));
			}
		} else if (PyUnicode_Check(obj)) {
			tmp = PyUnicode_AsUTF8String(obj);
			if (tmp != NULL) {
				utf8 = PyString_AsString(tmp);
				if (chars == -1) {
					chars = static_cast<int>(PyUnicode_GET_SIZE(obj));
				}
			}
		}
		if (utf8 != NULL) {
			rmmseg::Word *w = rmmseg::make_word(utf8, chars, freq, 
				static_cast<int>(strlen(utf8)));
			rmmseg::dict::add(w);
		}
	}

	Py_XDECREF(tmp);
	Py_INCREF(Py_None);
	return (PyObject *)Py_None;
}

static PyObject *
mmseg_Dictionary_has_word(PyObject *self, PyObject *obj)
{
	char *utf8 = NULL;
	PyObject *uni = NULL;

	if (PyString_Check(obj)) {
		/* A plain ASCII string is also a valid UTF-8 string */
		utf8 = PyString_AsString(obj);
	} else if (PyUnicode_Check(obj)) {
		uni = PyUnicode_AsUTF8String(obj);
		if (uni != NULL) {
			utf8 = PyString_AsString(uni);
		}
	}
	if (utf8 != NULL) {
		if (rmmseg::dict::get(utf8, static_cast<int>(strlen(utf8)))) {
			Py_XDECREF(uni);
			Py_INCREF(Py_True);
			return (PyObject *)Py_True;
		}
	}

	Py_XDECREF(uni);
	Py_INCREF(Py_False);
	return (PyObject *)Py_False;
}

static PyMethodDef mmseg_Dictionary_methods[] = {
	{"load_chars", (PyCFunction)mmseg_Dictionary_load_chars, METH_VARARGS | METH_STATIC, "Load a characters dictionary from a file."},
	{"load_words", (PyCFunction)mmseg_Dictionary_load_words, METH_VARARGS | METH_STATIC, "Load a words dictionary from a file."},
	{"add", (PyCFunction)mmseg_Dictionary_add, METH_KEYWORDS | METH_STATIC, "Add a word to the in-memory dictionary."},
	{"has_word", (PyCFunction)mmseg_Dictionary_has_word, METH_O | METH_STATIC, "Check whether one word is included in the dictionary."},
	{NULL, NULL, 0, NULL}        /* Sentinel */
};


static PyTypeObject mmseg_DictionaryType = {
	PyObject_HEAD_INIT(NULL)
	0,                                         /* ob_size */
	"mmseg.Dictionary",                        /* tp_name */
	sizeof(mmseg_Dictionary),                  /* tp_basicsize */
	0,                                         /* tp_itemsize */
	0,                                         /* tp_dealloc */
	0,                                         /* tp_print */
	0,                                         /* tp_getattr */
	0,                                         /* tp_setattr */
	0,                                         /* tp_compare */
	0,                                         /* tp_repr */
	0,                                         /* tp_as_number */
	0,                                         /* tp_as_sequence */
	0,                                         /* tp_as_mapping */
	0,                                         /* tp_hash  */
	0,                                         /* tp_call */
	0,                                         /* tp_str */
	0,                                         /* tp_getattro */
	0,                                         /* tp_setattro */
	0,                                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,  /* tp_flags */
	"MMSeg Dictionary.",                       /* tp_doc */
	0,                                         /* tp_traverse */
	0,                                         /* tp_clear */
	0,                                         /* tp_richcompare */
	0,                                         /* tp_weaklistoffset */
	0,                                         /* tp_iter: __iter__() method */
	0,                                         /* tp_iternext: next() method */
	mmseg_Dictionary_methods,                  /* tp_methods */
	0,                                         /* tp_members */
	0,                                         /* tp_getset */
	0,                                         /* tp_base */
	0,                                         /* tp_dict */
	0,                                         /* tp_descr_get */
	0,                                         /* tp_descr_set */
	0,                                         /* tp_dictoffset */
	0,                                         /* tp_init */
	0,                                         /* tp_alloc */
	0                                          /* tp_new */
};


/* Token */

typedef struct {
	PyObject_HEAD
	PyObject *text;
	int start;
	int end;
	int length;
} mmseg_Token;

static PyObject *
mmseg_Token_str(mmseg_Token* self)
{
	Py_INCREF(self->text);
	return self->text;
}

static PyObject *
mmseg_Token_repr(mmseg_Token* self)
{
	PyObject *repr;
	repr = PyString_FromString("");
	PyString_ConcatAndDel(&repr, PyString_FromFormat("<Token %d..%d ", self->start, self->end));
	PyString_ConcatAndDel(&repr, PyObject_Repr(self->text));
	PyString_ConcatAndDel(&repr, PyString_FromString(">"));
	return repr;
}

static void
mmseg_Token_dealloc(mmseg_Token* self)
{
	Py_XDECREF(self->text);
	self->ob_type->tp_free((PyObject*)self);

	#ifdef DEBUG
		PySys_WriteStderr("MMSeg Token object destroyed!\n");
	#endif
}

static mmseg_Token*
mmseg_Token_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	mmseg_Token *self;

	self = (mmseg_Token *)type->tp_alloc(type, 0);
	if (self != NULL) {
		self->text = PyString_FromString("");
		if (self->text == NULL) {
			Py_DECREF(self);
			return NULL;
		}
		self->start = 0;
		self->end = 0;
		self->length = 0;
	}

	#ifdef DEBUG
		PySys_WriteStderr("MMSeg Token object created! (%lu)\n", sizeof(mmseg_Token));
	#endif

	return self;
}

static int
mmseg_Token_init(mmseg_Token *self, PyObject *args, PyObject *kwds)
{
	char *utf8 = NULL;
	PyObject *obj = NULL, *uni = NULL, *tmp;
	static const char *kwlist[] = {"text", "start", NULL};

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|Oi", (char **)(kwlist), &obj, &self->start)) {
		return -1;
	}

	if (obj) {
		if (PyString_Check(obj)) {
			/* A plain ASCII string is also a valid UTF-8 string */
			utf8 = PyString_AsString(obj);
			uni = PyUnicode_DecodeUTF8(utf8, strlen(utf8), "strict");
		} else if (PyUnicode_Check(obj)) {
			uni = obj;
			Py_INCREF(uni);
		}

		tmp = self->text;
		self->text = uni;
		self->length = static_cast<int>(PyUnicode_GET_SIZE(self->text));
		self->end = self->start + self->length;
		Py_XDECREF(tmp);
	}

	#ifdef DEBUG
		PySys_WriteStderr("MMSeg Token object initialized!\n");
	#endif

	return 0;
}

static PyMemberDef mmseg_Token_members[] = {
	{(char *)"text", T_OBJECT_EX, offsetof(mmseg_Token, text), READONLY},
	{(char *)"start", T_INT, offsetof(mmseg_Token, start), READONLY},
	{(char *)"end", T_INT, offsetof(mmseg_Token, end), READONLY},
	{(char *)"length", T_INT, offsetof(mmseg_Token, length), READONLY},
	{NULL}        /* Sentinel */
};

/* Type definition */

static PyTypeObject mmseg_TokenType = {
	PyObject_HEAD_INIT(NULL)
	0,                                         /* ob_size */
	"mmseg.Token",                             /* tp_name */
	sizeof(mmseg_Token),                       /* tp_basicsize */
	0,                                         /* tp_itemsize */
	(destructor)mmseg_Token_dealloc,           /* tp_dealloc */
	0,                                         /* tp_print */
	0,                                         /* tp_getattr */
	0,                                         /* tp_setattr */
	0,                                         /* tp_compare */
	(reprfunc)mmseg_Token_repr,                /* tp_repr */
	0,                                         /* tp_as_number */
	0,                                         /* tp_as_sequence */
	0,                                         /* tp_as_mapping */
	0,                                         /* tp_hash  */
	0,                                         /* tp_call */
	(reprfunc)mmseg_Token_str,                 /* tp_str */
	0,                                         /* tp_getattro */
	0,                                         /* tp_setattro */
	0,                                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,  /* tp_flags */
	"MMSeg Token object.",                     /* tp_doc */
	0,                                         /* tp_traverse */
	0,                                         /* tp_clear */
	0,                                         /* tp_richcompare */
	0,                                         /* tp_weaklistoffset */
	0,                                         /* tp_iter: __iter__() method */
	0,                                         /* tp_iternext: next() method */
	0,                                         /* tp_methods */
	mmseg_Token_members,                       /* tp_members */
	0,                                         /* tp_getset */
	0,                                         /* tp_base */
	0,                                         /* tp_dict */
	0,                                         /* tp_descr_get */
	0,                                         /* tp_descr_set */
	0,                                         /* tp_dictoffset */
	(initproc)mmseg_Token_init,                /* tp_init */
	0,                                         /* tp_alloc */
	(newfunc)mmseg_Token_new                   /* tp_new */
};


/* Algorithm */

typedef struct {
	PyObject_HEAD
	char *text;
	int utf8;
	rmmseg::Algorithm *algorithm;
} mmseg_Algorithm;

static int
mmseg_Algorithm_init(mmseg_Algorithm *self, PyObject *args, PyObject *kwds)
{
	char *utf8 = NULL;
	PyObject *obj, *uni = NULL;

	if (!PyArg_ParseTuple(args, "O", &obj)) {
		return -1;
	}

	if (PyString_Check(obj)) {
		/* A plain ASCII string is also a valid UTF-8 string */
		utf8 = PyString_AsString(obj);
		/* verify it's valid UTF-8: */
		uni = PyUnicode_DecodeUTF8(utf8, strlen(utf8), "strict");
		if (uni == NULL) {
			utf8 = NULL;
		}
		self->utf8 = 1;
	} else if (PyUnicode_Check(obj)) {
		uni = PyUnicode_AsUTF8String(obj);
		if (uni != NULL) {
			utf8 = PyString_AsString(uni);
		}
		self->utf8 = 0;
	}
	if (utf8 != NULL) {
		self->text = PyMem_Strdup(utf8);
		self->algorithm = new rmmseg::Algorithm(self->text, 
			static_cast<int>(strlen(self->text)));
		#ifdef DEBUG
			PySys_WriteStderr("Algorithm object initialized!\n");
		#endif
		Py_XDECREF(uni);
		return 0;
	}

	Py_XDECREF(uni);
	return -1;
}

static void
mmseg_Algorithm_dealloc(mmseg_Algorithm *self)
{
	if (self->text != NULL) PyMem_Del(self->text);
	if (self->algorithm != NULL) delete self->algorithm;

	self->ob_type->tp_free((PyObject*)self);

	#ifdef DEBUG
		PySys_WriteStderr("Algorithm object destroyed!\n");
	#endif
}

mmseg_Algorithm*
mmseg_Algorithm_iter(mmseg_Algorithm *self)
{
	Py_INCREF(self);
	return self;
}

mmseg_Token*
mmseg_Algorithm_iternext(mmseg_Algorithm *self)
{
	PyObject *uni = NULL;
	mmseg_Token *result = NULL;

	rmmseg::Token rtk = self->algorithm->next_token();

	if (rtk.text != NULL) {
		result = PyObject_New(mmseg_Token, &mmseg_TokenType);
		if (result) {
			if (self->utf8) {
				if (!(result->text = PyString_FromStringAndSize(rtk.text, rtk.length))) {
					Py_DECREF(result);
					return NULL;
				}
				result->start = static_cast<int>(rtk.text - self->algorithm->get_text());
				result->length = rtk.length;
				result->end = result->start + result->length;
				return result;
			} else {
				if (!(result->text = PyUnicode_DecodeUTF8(rtk.text, rtk.length, "strict"))) {
					Py_DECREF(result);
					return NULL;
				}
				if (!(uni = PyUnicode_DecodeUTF8(self->algorithm->get_text(), rtk.text - self->algorithm->get_text(), "strict"))) {
					Py_DECREF(result);
					return NULL;
				}
				result->start = static_cast<int>(PyUnicode_GET_SIZE(uni));
				result->length = static_cast<int>(PyUnicode_GET_SIZE(result->text));
				result->end = result->start + result->length;
				Py_DECREF(uni);
				return result;
			}
		}
	}

	/* Raising of standard StopIteration exception with empty value. */
	PyErr_SetNone(PyExc_StopIteration);
	return NULL;
}

/* Type definition */

static PyTypeObject mmseg_AlgorithmType = {
	PyObject_HEAD_INIT(NULL)
	0,                                         /* ob_size */
	"mmseg.Algorithm",                         /* tp_name */
	sizeof(mmseg_Algorithm),                   /* tp_basicsize */
	0,                                         /* tp_itemsize */
	(destructor)mmseg_Algorithm_dealloc,       /* tp_dealloc */
	0,                                         /* tp_print */
	0,                                         /* tp_getattr */
	0,                                         /* tp_setattr */
	0,                                         /* tp_compare */
	0,                                         /* tp_repr */
	0,                                         /* tp_as_number */
	0,                                         /* tp_as_sequence */
	0,                                         /* tp_as_mapping */
	0,                                         /* tp_hash  */
	0,                                         /* tp_call */
	0,                                         /* tp_str */
	0,                                         /* tp_getattro */
	0,                                         /* tp_setattro */
	0,                                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_ITER, /* tp_flags */
	"MMSeg Algorithm iterator object.",        /* tp_doc */
	0,                                         /* tp_traverse */
	0,                                         /* tp_clear */
	0,                                         /* tp_richcompare */
	0,                                         /* tp_weaklistoffset */
	(getiterfunc)mmseg_Algorithm_iter,         /* tp_iter: __iter__() method */
	(iternextfunc)mmseg_Algorithm_iternext,    /* tp_iternext: next() method */
	0,                                         /* tp_methods */
	0,                                         /* tp_members */
	0,                                         /* tp_getset */
	0,                                         /* tp_base */
	0,                                         /* tp_dict */
	0,                                         /* tp_descr_get */
	0,                                         /* tp_descr_set */
	0,                                         /* tp_dictoffset */
	(initproc)mmseg_Algorithm_init,            /* tp_init */
};


/* Python constructor */

/* Module functions */

static PyMethodDef mmseg_methods[] = {
	{NULL, NULL, 0, NULL}        /* Sentinel */
};


/* Module init function */

PyMODINIT_FUNC
init_mmseg(void)
{
	PyObject* m;

	mmseg_DictionaryType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&mmseg_DictionaryType) < 0)
		return;

	mmseg_TokenType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&mmseg_TokenType) < 0)
		return;

	mmseg_AlgorithmType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&mmseg_AlgorithmType) < 0)
		return;

	m = Py_InitModule("_mmseg", mmseg_methods);

	Py_INCREF(&mmseg_DictionaryType);
	PyModule_AddObject(m, "Dictionary", (PyObject *)&mmseg_DictionaryType);

	Py_INCREF(&mmseg_TokenType);
	PyModule_AddObject(m, "Token", (PyObject *)&mmseg_TokenType);

	Py_INCREF(&mmseg_AlgorithmType);
	PyModule_AddObject(m, "Algorithm", (PyObject *)&mmseg_AlgorithmType);
}
