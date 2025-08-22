#include <iostream>
#include <memory>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <cmath>

enum class OperationTypes
{
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
    // Binary apply
    virtual double apply(double a, double b) const {
        throw std::runtime_error("Binary apply not implemented!");
    }

    // Unary apply
    virtual double apply(double a) const {
        throw std::runtime_error("Unary apply not implemented!");
    }

    virtual ~IOperation() = default;
};

// -------------------- Concrete Strategies --------------------
class AddOperation : public IOperation {
public:
    double apply(double a, double b) const override {
        return a + b;
    }
};

class SubtractOperation : public IOperation {
public:
    double apply(double a, double b) const override {
        return a - b;
    }
};

class MultiplyOperation : public IOperation {
public:
    double apply(double a, double b) const override {
        return a * b;
    }
};

class DivideOperation : public IOperation {
public:
    double apply(double a, double b) const override {
        if (b == 0) throw std::runtime_error("Division by zero!");
        return a / b;
    }
};

class PowerOperation : public IOperation {
public:
    double apply(double a, double b) const override {
        return std::pow(a, b);
    }
};

class SqrtOperation : public IOperation {
public:
    double apply(double a) const override {
        if (a < 0) throw std::runtime_error("Square root of negative number!");
        return std::sqrt(a);
    }
};

// -------------------- Calculator --------------------
class Calculator {
private:
    std::unordered_map<OperationTypes, std::unique_ptr<IOperation>> operations;

public:
    Calculator() {
        registerOperation(OperationTypes::Add, std::make_unique<AddOperation>());
        registerOperation(OperationTypes::Sub, std::make_unique<SubtractOperation>());
        registerOperation(OperationTypes::Mul, std::make_unique<MultiplyOperation>());
        registerOperation(OperationTypes::Div, std::make_unique<DivideOperation>());
        registerOperation(OperationTypes::Pow, std::make_unique<PowerOperation>());
        registerOperation(OperationTypes::Sqrt, std::make_unique<SqrtOperation>());
    }

    void registerOperation(const OperationTypes& opType, std::unique_ptr<IOperation> op) {
        operations[opType] = std::move(op);
    }

    // Variadic template for binary operations
    template<typename... Args>
    double calculate(const OperationTypes& opType, Args... args) {
        auto it = operations.find(opType);
        if (it == operations.end()) {
            throw std::invalid_argument("Operation not found!");
        }

        if constexpr (sizeof...(args) == 1) {
            // Unary case
            return applyUnary(*it->second, args...);
        } else {
            // Binary case
            return applyBinary(*it->second, args...);
        }
    }

private:
    // Unary apply
    template<typename T>
    double applyUnary(const IOperation& op, T value) {
        return op.apply(static_cast<double>(value));
    }

    // Binary apply with left fold
    template<typename T, typename... Rest>
    double applyBinary(const IOperation& op, T first, Rest... rest) {
        double result = static_cast<double>(first);
        ((result = op.apply(result, static_cast<double>(rest))), ...);
        return result;
    }
};

// -------------------- Example Usage --------------------
int main() {
    Calculator calc;

    std::cout << "Add: " << calc.calculate(OperationTypes::Add, 1, 2, 3, 4, 5) << "\n";   // 15
    std::cout << "Mul: " << calc.calculate(OperationTypes::Mul, 2, 3, 4) << "\n";         // 24
    std::cout << "Sub: " << calc.calculate(OperationTypes::Sub, 20, 5, 3) << "\n";        // 12
    std::cout << "Div: " << calc.calculate(OperationTypes::Div, 100, 2, 5) << "\n";       // 10
    std::cout << "Pow: " << calc.calculate(OperationTypes::Pow, 2, 3) << "\n";            // 8
    std::cout << "Sqrt: " << calc.calculate(OperationTypes::Sqrt, 16) << "\n";            // 4

    // Add a new operation at runtime
    class ModuloOperation : public IOperation {
    public:
        double apply(double a, double b) const override {
            if (b == 0) throw std::runtime_error("Modulo by zero!");
            return std::fmod(a, b);
        }
    };

    calc.registerOperation(OperationTypes::Mod, std::make_unique<ModuloOperation>());
    std::cout << "Mod: " << calc.calculate(OperationTypes::Mod, 20, 6) << "\n";           // 2

    return 0;
}
