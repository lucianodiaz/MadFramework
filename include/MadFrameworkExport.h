// MadExport.h
#pragma once

#if defined(MADFRAMEWORK_BUILD_DLL)
#define MAD_API __declspec(dllexport)
#elif defined(MADFRAMEWORK_USE_DLL)
#define MAD_API __declspec(dllimport)
#else
  // Build/uso ESTÁTICO
#define MAD_API
#endif