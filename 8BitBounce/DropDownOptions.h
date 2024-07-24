#pragma once
#include "WorldWindow.h"
#include "UiSettings.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>

void DropDownOptions(HWND parent, WindowPhysics& rigidBody);

std::wstring FormatLargeNumber(float number);
