#pragma once


#ifdef MADFRAMEWORK_EXPORTS
// Cuando se compila la librer�a
#define MAD_API __declspec(dllexport)
#else
// Cuando se usa desde otro proyecto
#define MAD_API __declspec(dllimport)
#endif
