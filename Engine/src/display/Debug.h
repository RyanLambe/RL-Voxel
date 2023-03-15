#pragma once
#include "Debuger.h"

#define log(out) Display::Debuger::log(out);
#define logError(out) Display::Debuger::logError(out, __LINE__, __FILE__)
#define logErrorCode(out) Display::Debuger::logErrorCode(out, __LINE__, __FILE__)
#define logWarning(out) Display::Debuger::logWarning(out);