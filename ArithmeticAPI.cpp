#include "ArithmeticAPI.h"
#include <string>
#include <memory>
#include <limits>
#include <cstring>

// ==================== OOP core ============================

// Base class: demonstrates inheritance & polymorphism
class Operation
{
public:
    virtual ~Operation() = default;
    virtual double Execute(double val1, double val2) const = 0;
    virtual const char* Name() const = 0;
};

// Derived operations (runtime polymorphism)
class AddOperation : public Operation
{
public:
    double Execute(double val1, double val2) const override { return val1 + val2; }
    const char* Name() const override { return "Add"; }
};

class SubOperation : public Operation
{
public:
    double Execute(double val1, double val2) const override { return val1 - val2; }
    const char* Name() const override { return "Sub"; }
};

class MulOperation : public Operation
{
public:
    double Execute(double val1, double val2) const override { return val1 * val2; }
    const char* Name() const override { return "Mul"; }
};

class DivOperation : public Operation
{
public:
    double Execute(double val1, double val2) const override
    {
        return (val2 == 0.0) ? std::numeric_limits<double>::quiet_NaN() : (val1 / val2);
    }
    const char* Name() const override { return "Div"; }
};

// A simple factory to create the right derived type
static std::unique_ptr<Operation> MakeOperation(OperationType type)
{
    switch (type)
    {
    case OperationType::Add: return std::make_unique<AddOperation>();
    case OperationType::Sub: return std::make_unique<SubOperation>();
    case OperationType::Mul: return std::make_unique<MulOperation>();
    case OperationType::Div: return std::make_unique<DivOperation>();
    default:                 return std::make_unique<AddOperation>();
    }
}

// Calculator “owns” an Operation and exposes getters/setters
class Calculator
{
public:
    Calculator()
        : OpType(OperationType::Add)
        , Op(MakeOperation(OpType))
        , NameUtf8("ArithmeticAPI")
    {
    }

    // Getter/Setter for operation (shows encapsulation)
    OperationType GetOperation() const { return OpType; }
    void SetOperation(OperationType type)
    {
        OpType = type;
        Op = MakeOperation(type); // swap polymorphic behavior at runtime
    }

    // Name getter/setter
    const std::string& GetName() const { return NameUtf8; }
    void SetName(const char* utf8)
    {
        if (utf8) NameUtf8 = utf8;
        else      NameUtf8.clear();
    }

    // Delegate compute to polymorphic Operation
    double Compute(double val1, double val2) const
    {
        return Op->Execute(val1, val2);
    }

private:
    OperationType OpType;
    std::unique_ptr<Operation> Op;
    std::string NameUtf8;
};

// ================== C API (exports) =======================

ARITH_API CalculatorHandle ARITH_CALL CreateCalculator()
{
    // Allocate and return opaque pointer
    return reinterpret_cast<CalculatorHandle>(new Calculator());
}

ARITH_API void ARITH_CALL DestroyCalculator(CalculatorHandle handle)
{
    if (!handle) return;
    auto* calc = reinterpret_cast<Calculator*>(handle);
    delete calc;
}

ARITH_API void ARITH_CALL SetOperation(CalculatorHandle handle, OperationType type)
{
    if (!handle) return;
    auto* calc = reinterpret_cast<Calculator*>(handle);
    calc->SetOperation(type);
}

ARITH_API int ARITH_CALL GetOperation(CalculatorHandle handle)
{
    if (!handle) return -1;
    auto* calc = reinterpret_cast<Calculator*>(handle);
    return static_cast<int>(calc->GetOperation());
}

ARITH_API void ARITH_CALL SetName(CalculatorHandle handle, const char* nameUtf8)
{
    if (!handle) return;
    auto* calc = reinterpret_cast<Calculator*>(handle);
    calc->SetName(nameUtf8);
}

ARITH_API int ARITH_CALL GetName(CalculatorHandle handle, char* outBuf, int bufSize)
{
    if (!handle) return 0;
    auto* calc = reinterpret_cast<Calculator*>(handle);
    const std::string& s = calc->GetName();

    if (outBuf == nullptr || bufSize <= 0)
    {
        // required size (excluding null terminator)
        return static_cast<int>(s.size());
    }

    // Copy up to bufSize - 1 and null-terminate
    int toCopy = static_cast<int>(s.size());
    if (toCopy > bufSize - 1) toCopy = bufSize - 1;

    if (toCopy > 0)
        std::memcpy(outBuf, s.data(), static_cast<std::size_t>(toCopy));

    outBuf[toCopy] = '\0';
    return toCopy;
}

ARITH_API double ARITH_CALL Compute(CalculatorHandle handle, double val1, double val2)
{
    if (!handle) return std::numeric_limits<double>::quiet_NaN();
    auto* calc = reinterpret_cast<Calculator*>(handle);
    return calc->Compute(val1, val2);
}


/**
 * Helper function: UnitFactorToCm
 * Returns how many centimeters = 1 unit of the given type.
 */
static inline bool UnitFactorToCm(int unit, double& toCm)
{
    switch (unit)
    {
        case Unit_Miles:
            // set toCm for miles (hint: 1 mile = 160934.4 cm)
            toCm = 160934.4;
            return true;

        case Unit_Feet:
            // set toCm for feet (hint: 1 foot = 30.48 cm)
            toCm = 30.48;
            return true;

        case Unit_Centimeters:
            // set toCm for centimeters
            toCm = 1.0;
            return true;

        default:
            return false; // Unknown unit
    }
}

/**
 * ConvertUnits
 * Converts value from "fromUnit" into centimeters,
 * then converts to "toUnit".
 */
ARITH_API double ARITH_CALL ConvertUnits(double value, int fromUnit, int toUnit)
{
    double fFrom = 0.0, fTo = 0.0;

    // Step 1: Get cm factor for source unit
    if (!UnitFactorToCm(fromUnit, fFrom))
        return std::numeric_limits<double>::quiet_NaN();

    // Step 2: Get cm factor for target unit
    if (!UnitFactorToCm(toUnit, fTo))
        return std::numeric_limits<double>::quiet_NaN();

    // Step 3: Conversion math
    double inCm = value * fFrom;
    return inCm / fTo;

    return std::numeric_limits<double>::quiet_NaN(); // placeholder
}