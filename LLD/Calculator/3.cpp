#include <iostream>
#include <memory>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <cmath>

// -------------------- Enum --------------------
enum class OperationTypes {
    Add,
    Sub,
    Mul,
    Div,
    Pow,
    Sqrt,
    Mod
};

// -------------------- Strategy Interface --------------------
class IOperation {
public:
    virtual ~IOperation() = default;
};

// -------------------- Concrete Strategies --------------------
class AddOperation : public IOperation {
public:
    static constexpr int arity = -1; // variadic
    double apply(double a, double b) const { return a + b; }
};

class SubtractOperation : public IOperation {
public:
    static constexpr int arity = -1; // variadic
    double apply(double a, double b) const { return a - b; }
};

class MultiplyOperation : public IOperation {
public:
    static constexpr int arity = -1; // variadic
    double apply(double a, double b) const { return a * b; }
};

class DivideOperation : public IOperation {
public:
    static constexpr int arity = -1; // variadic
    double apply(double a, double b) const {
        if (b == 0) throw std::runtime_error("Division by zero!");
        return a / b;
    }
};

class PowerOperation : public IOperation {
public:
    static constexpr int arity = 2; // strictly binary
    double apply(double a, double b) const { return std::pow(a, b); }
};

class SqrtOperation : public IOperation {
public:
    static constexpr int arity = 1; // strictly unary
    double apply(double a) const {
        if (a < 0) throw std::runtime_error("Square root of negative number!");
        return std::sqrt(a);
    }
};

class ModuloOperation : public IOperation {
public:
    static constexpr int arity = 2; // strictly binary
    double apply(double a, double b) const {
        if (b == 0) throw std::runtime_error("Modulo by zero!");
        return std::fmod(a, b);
    }
};

// -------------------- Calculator --------------------
template <typename Op>
class CalculatorImpl {
public:
    template <typename... Args>
    double calculate(Args... args) const {
        constexpr int expected = Op::arity;
        constexpr int given = sizeof...(args);

        if constexpr (expected == 1) {
            static_assert(given == 1, "Unary operation requires exactly 1 argument");
            return applyUnary(args...);
        } 
        else if constexpr (expected == 2) {
            static_assert(given == 2, "Binary operation requires exactly 2 arguments");
            return applyBinary(args...);
        } 
        else if constexpr (expected == -1) {
            static_assert(given >= 2, "Variadic operation requires at least 2 arguments");
            return applyVariadic(args...);
        }
    }

private:
    // Unary apply
    template <typename T>
    double applyUnary(T value) const {
        Op op;
        return op.apply(static_cast<double>(value));
    }

    // Binary apply
    template <typename T1, typename T2>
    double applyBinary(T1 a, T2 b) const {
        Op op;
        return op.apply(static_cast<double>(a), static_cast<double>(b));
    }

    // Variadic apply with fold
    template <typename T, typename... Rest>
    double applyVariadic(T first, Rest... rest) const {
        Op op;
        double result = static_cast<double>(first);
        ((result = op.apply(result, static_cast<double>(rest))), ...);
        return result;
    }
};

// -------------------- Example Usage --------------------
int main() {
    CalculatorImpl<AddOperation> addCalc;
    CalculatorImpl<MultiplyOperation> mulCalc;
    CalculatorImpl<SubtractOperation> subCalc;
    CalculatorImpl<DivideOperation> divCalc;
    CalculatorImpl<PowerOperation> powCalc;
    CalculatorImpl<SqrtOperation> sqrtCalc;
    CalculatorImpl<ModuloOperation> modCalc;

    std::cout << "Add: "   << addCalc.calculate(1, 2, 3, 4, 5) << "\n";   // 15
    std::cout << "Mul: "   << mulCalc.calculate(2, 3, 4) << "\n";         // 24
    std::cout << "Sub: "   << subCalc.calculate(20, 5, 3) << "\n";        // 12
    std::cout << "Div: "   << divCalc.calculate(100, 2, 5) << "\n";       // 10
    std::cout << "Pow: "   << powCalc.calculate(2, 3) << "\n";            // 8
    std::cout << "Sqrt: "  << sqrtCalc.calculate(16) << "\n";             // 4
    std::cout << "Mod: "   << modCalc.calculate(20, 6) << "\n";           // 2

    // ❌ Compile-time error (unary op with 2 args)
    // std::cout << sqrtCalc.calculate(16, 4) << "\n";

    return 0;
}
