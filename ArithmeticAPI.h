#pragma once
#include <cstddef>

#ifdef _WIN32
#define ARITH_API extern "C" __declspec(dllexport)
#define ARITH_CALL __cdecl
#else
#define ARITH_API extern "C"
#define ARITH_CALL
#endif

// --------- Opaque handle for consumers (C#, UE) ----------
typedef void* CalculatorHandle;

// --------- Operation type for switching at runtime --------
enum class OperationType : int
{
    Add = 0,
    Sub = 1,
    Mul = 2,
    Div = 3
};

// ------------------- Exported C API -----------------------
ARITH_API CalculatorHandle ARITH_CALL CreateCalculator();
ARITH_API void             ARITH_CALL DestroyCalculator(CalculatorHandle handle);

// Set/Get which operation is active (Add/Sub/Mul/Div)
ARITH_API void ARITH_CALL SetOperation(CalculatorHandle handle, OperationType type);
ARITH_API int  ARITH_CALL GetOperation(CalculatorHandle handle); // returns (int)OperationType

// Optional: a name property to demo getters/setters
// SetName expects UTF-8; pass nullptr to clear
ARITH_API void ARITH_CALL SetName(CalculatorHandle handle, const char* nameUtf8);

// GetName copies UTF-8 into outBuf. Returns number of bytes written (excluding null terminator).
// If outBuf == nullptr or bufSize == 0, returns required size (excluding null terminator).
ARITH_API int  ARITH_CALL GetName(CalculatorHandle handle, char* outBuf, int bufSize);

// Compute using current operation; Div by zero -> NaN
ARITH_API double ARITH_CALL Compute(CalculatorHandle handle, double val1, double val2);


// ------------------ Unit Conversion ------------------

/**
 * UnitType enum
 * Each unit represents a measurement of distance.
 * We start with Miles, Feet, and Centimeters.
 */
 
enum UnitType : int
{
    Unit_Miles = 0,
    Unit_Feet = 1,
    Unit_Centimeters = 2
};


/**
 * ConvertUnits
 * Converts a value from one unit to another.
 * You will complete the implementation in ArithmeticAPI.cpp.
 */
ARITH_API double ARITH_CALL ConvertUnits(double value, int fromUnit, int toUnit);