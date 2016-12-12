#pragma once
#ifndef _DLL_ALPHABET_H
#define _DLL_ALPHABET_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _DLL_ALPHABET
#define _DLL_ALPHABET_API __declspec(dllexport)
#else
#define _DLL_ALPHABET_API __declspec(dllimport)
#endif

	_DLL_ALPHABET_API void AlphaDLL(int);

#undef _DLL_ALPHABET_API

#ifdef __cplusplus
}
#endif

#endif