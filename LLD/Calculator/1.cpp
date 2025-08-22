#include <iostream>
#include <memory>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <cmath>

// -------------------- Strategy Interface --------------------
class IOperation {
public:
    virtual double apply(double a, double b) const = 0;
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

// -------------------- Calculator --------------------
class Calculator {
private:
    std::unordered_map<std::string, std::unique_ptr<IOperation>> operations;

public:
    Calculator() {
        registerOperation("add", std::make_unique<AddOperation>());
        registerOperation("sub", std::make_unique<SubtractOperation>());
        registerOperation("mul", std::make_unique<MultiplyOperation>());
        registerOperation("div", std::make_unique<DivideOperation>());
        registerOperation("pow", std::make_unique<PowerOperation>());
    }

    void registerOperation(const std::string& name, std::unique_ptr<IOperation> op) {
        operations[name] = std::move(op);
    }

    template<typename... Args>
    double calculate(const std::string& opName, Args... args) {
        auto it = operations.find(opName);
        if (it == operations.end()) {
            throw std::invalid_argument("Operation not found!");
        }
        return applyOperation(*it->second, args...);
    }

private:
    // Base case
    template<typename T>
    double applyOperation(const IOperation&, T value) {
        return static_cast<double>(value);
    }

    // Recursive variadic expansion
    template<typename T, typename... Rest>
    double applyOperation(const IOperation& op, T first, Rest... rest) {
        return op.apply(static_cast<double>(first), applyOperation(op, rest...));
    }
};


// -------------------- Example Usage --------------------
int main() {
    Calculator calc;

    std::cout << "Add: " << calc.calculate("add", 1, 2, 3, 4, 5) << "\n";   // 15
    std::cout << "Mul: " << calc.calculate("mul", 2, 3, 4) << "\n";         // 24
    std::cout << "Sub: " << calc.calculate("sub", 20, 5, 3) << "\n";        // 12
    std::cout << "Div: " << calc.calculate("div", 100, 2, 5) << "\n";       // 10
    std::cout << "Pow: " << calc.calculate("pow", 2, 3) << "\n";            // 8

    // You can add new operations without modifying Calculator
    class ModuloOperation : public IOperation {
    public:
        double apply(double a, double b) const override {
            return std::fmod(a, b);
        }
    };

    calc.registerOperation("mod", std::make_unique<ModuloOperation>());
    std::cout << "Mod: " << calc.calculate("mod", 20, 6) << "\n";           // 2

    return 0;
}
