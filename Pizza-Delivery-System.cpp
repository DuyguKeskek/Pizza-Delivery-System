#include <iostream>
#include <vector>

using namespace std;

class Observer;

class Pizza;

class Drink;

class Command;

class PizzaCompanyFactory;

// Abstract subject class
class Subject {
public:
    virtual void Attach(Observer *observer) = 0;

    virtual void Detach(Observer *observer) = 0;

    virtual void Notify() = 0;

protected:
    vector<Observer *> _observers;
};

// Abstract Observer class
class Observer {
public:
    virtual void Update(PizzaCompanyFactory *companyFactory) = 0;
};

// Abstract Factory and Concrete Subject
class PizzaCompanyFactory : public Subject {
public:
    virtual Pizza *CreatePizza() = 0;

    virtual Drink *CreateDrink() = 0;

    virtual string GetCompanyName() = 0;

    virtual int GetDistance() = 0;

    void Action(int type) {
        if (type == 0) {
            CreatePizza();
        } else {
            CreateDrink();
        }
    }

    void Attach(Observer *observer) {
        _observers.push_back(observer);
    }

    void Detach(Observer *observer) {
        int count = _observers.size();
        int i;

        for (i = 0; i < count; i++) {
            if (_observers[i] == observer)
                break;
        }
        if (i < count)
            _observers.erase(_observers.begin() + i);
    }

    void Notify() {
        // set argument to something that helps
        // tell the Observers what happened
        int count = _observers.size();
        for (int i = 0; i < count; i++) {
            (_observers[i])->Update(this);
        }
    }

    void SendReport() {
        // Whenever a change happens to _price, notify
        // observers.
        Notify();
    }
};

// Command Begin
class Command {
public:
    virtual void Execute() = 0;

protected:
    Command() { };
};

class PizzaRequestCommand : public Command {
public:
    PizzaRequestCommand(PizzaCompanyFactory *companyFactory) : _factory(companyFactory) { }

    void Execute() {
        _factory->Action(0);
    }

private:
    PizzaCompanyFactory *_factory;
};

class DrinkRequestCommand : public Command {
public:

    DrinkRequestCommand(PizzaCompanyFactory *factory) : _factory(factory) { }

    void Execute() {
        _factory->Action(1);
    }

private:
    PizzaCompanyFactory *_factory;
};

// Invoker
class TruckDriver {
private:
    vector<Command *> _commands;
    unsigned int _orderCount = 0;

protected:
    vector<PizzaCompanyFactory *> _companies;

public:
    /**
     * Return TruckDriver instead void.
     */
    TruckDriver *TakeOrder(Command *command, PizzaCompanyFactory *factory) {
        command->Execute();
        _commands.push_back(command);
        _companies.push_back(factory);
        _orderCount++;
        cout << "Order received, total: " << _orderCount << endl;
        return this;
    }

    // Template method.
    void ManageOrder() {
        GetCoordinates();
        CalculateRoute();
        DisplayRoute();
    }

protected:
    virtual void DisplayRoute() = 0;

    virtual void GetCoordinates() = 0;

    virtual vector<PizzaCompanyFactory *> CalculateRoute() = 0;
};

class DayTruckDriver : public TruckDriver {
public:

    DayTruckDriver() {
        cout << "DayTruckDriver" << endl;
    }

protected:
    void GetCoordinates() {
        for (int i = 0; i < _companies.size(); ++i) {
            cout << "Getting coordinates" << endl;
        }
    }

    vector<PizzaCompanyFactory *> CalculateRoute() {
        GEN();
        TSP();
        return _companies;
    }

    void DisplayRoute() {
        int smallest = INT_MAX;
        for (int i = 0; i < CalculateRoute().size(); ++i) {
            if (CalculateRoute()[i]->GetDistance() < smallest) {
                smallest = CalculateRoute()[i]->GetDistance();
            }
        }
        cout << "Shortest route is " << smallest << endl;
    }

private:
    void GEN() {
        cout << "Calling GEN" << endl;
    }

    void TSP() {
        cout << "Calling TSP" << endl;
    }
};

class NightTruckDriver : public TruckDriver {
public:

    NightTruckDriver() {
        cout << "NightTruckDriver" << endl;
    }

protected:
    void GetCoordinates() {
        for (std::vector<PizzaCompanyFactory *>::iterator it = _companies.begin(); it != _companies.end(); ++it) {
            cout << "Getting coordinates" << endl;
        }
    }

    vector<PizzaCompanyFactory *> CalculateRoute() {
        GEN();
        TSP();
        return _companies;
    }

    void DisplayRoute() {
        int smallest = INT_MAX;
        /*for(std::vector<PizzaCompanyFactory *>::iterator it = CalculateRoute().begin(); it != CalculateRoute().end(); ++it) {
            if ((*it)->GetDistance() < smallest) {
                smallest = (*it)->GetDistance();
            }
        }*/

        for (int i = 0; i < CalculateRoute().size(); ++i) {
            if (CalculateRoute()[i]->GetDistance() < smallest) {
                smallest = CalculateRoute()[i]->GetDistance();
            }
        }
        cout << "Shortest route is " << smallest << endl;
    }

private:
    void GEN() {
        cout << "Calling GEN" << endl;
    }

    void TSP() {
        cout << "Calling TSP" << endl;
    }
};

class DistributorCenter : public Observer {
private:
    string _investor_name;

    DistributorCenter() { }

    DistributorCenter(string name) : _investor_name(name) { };

    static DistributorCenter *instance;
public:

    static DistributorCenter *getInstance(string name) {
        if (instance == NULL) {
            instance = new DistributorCenter(name);
        }
        return instance;
    }

    void Update(PizzaCompanyFactory *companyFactory) {
        cout << "\nNotified" << "\n";
        Command *command = new PizzaRequestCommand(companyFactory);

        TruckDriver *driver;

        int number = rand() % 100 + 1; // generate random between 1-100
        if (number < 50) {
            driver = new DayTruckDriver();
        } else {
            driver = new NightTruckDriver();
        }

        // Small builder
        driver->TakeOrder(command, companyFactory);
        driver->ManageOrder();
    }
};

DistributorCenter *DistributorCenter::instance = NULL;

// Beginning Abstract Product Section
class Pizza {
};

class Drink {
};

// Beginning Concrete Product Section
class VeganPizza : public Pizza {
};

class NonVeganPizza : public Pizza {
};

class CokeDrink : public Drink {
};

class FantaDrink : public Drink {

};

class AyranDrink : public Drink {

};

class SodaDrink : public Drink {

};

// Beginning Concrete Factory
class PaninoBalcova : public PizzaCompanyFactory {
public:
    Pizza *CreatePizza() {
        cout << "Veganpizza is coming!!!" << endl;
        return new VeganPizza();
    }

    Drink *CreateDrink() {
        cout << "Coke is comingggg!!!" << endl;
        return new CokeDrink();
    }

    void AddVeganPizza(Pizza *pizza) {
        cout << "Adding pizza " << _veganPizzas.size() << endl;
        _veganPizzas.push_back(pizza);
    }

    void AddCokeDrink(Drink *drink) {
        _cokeDrinks.push_back(drink);
    }

    void ConsumePizza() {
        if (_veganPizzas.size() < _treshold) {
            SendReport();
            cout << "Sending Report" << endl;
        } else {
            _veganPizzas.erase(_veganPizzas.begin());
            cout << "Consuming " << _veganPizzas.size() << endl;
        }
    }

    string GetCompanyName() {
        return "Panino Balcova";
    }

    int GetDistance() {
        return 5;
    }

private:
    vector<Pizza *> _veganPizzas;
    vector<Drink *> _cokeDrinks;

    // the treshold value
    int _treshold = 3;
};

class PaninoKarsiyaka : public PizzaCompanyFactory {
public:
    Pizza *CreatePizza() {
        cout << "Veganpizza is coming!!!" << endl;
        return new VeganPizza();
    }

    Drink *CreateDrink() {
        cout << "Coke is comingggg!!!" << endl;
        return new CokeDrink();
    }

    void addNonVeganPizza(Pizza *pizza) {
        cout << "Adding pizza " << _veganPizzas.size() << endl;
        _veganPizzas.push_back(pizza);
    }

    void addFantaDrink(Drink *drink) {
        _fantaDrinks.push_back(drink);
    }

    void consumePizza() {
        if (_veganPizzas.size() < treshold) {
            SendReport();
            cout << "Sending Report" << endl;
            return;
        }

        _veganPizzas.erase(_veganPizzas.begin());
        cout << "Consuming " << _veganPizzas.size() << endl;
    }

    string GetCompanyName() {
        return "Panino Karsiyaka";
    }

    int GetDistance() {
        return 15;
    }

private:
    vector<Pizza *> _veganPizzas;
    vector<Drink *> _fantaDrinks;

    int treshold = 2;
};

class PaninoBayrakli : public PizzaCompanyFactory {
public:
    Pizza *CreatePizza() {
        cout << "Veganpizza is coming!!!" << endl;
        return new VeganPizza();
    }

    Drink *CreateDrink() {
        cout << "Coke is comingggg!!!" << endl;
        return new CokeDrink();
    }

    void addNonVeganPizza(Pizza *pizza) {
        cout << "Adding pizza " << _veganPizzas.size() << endl;
        _veganPizzas.push_back(pizza);
    }

    void addAyranDrink(Drink *drink) {
        _ayranDrinks.push_back(drink);
    }

    void consumePizza() {
        if (_veganPizzas.size() < treshold) {
            SendReport();
            cout << "Sending Report" << endl;
            return;
        }

        _veganPizzas.erase(_veganPizzas.begin());
        cout << "Consuming " << _veganPizzas.size() << endl;
    }

    string GetCompanyName() {
        return "Panino Bayrakli";
    }

    int GetDistance() {
        return 27;
    }

private:
    vector<Pizza *> _veganPizzas;
    vector<Drink *> _ayranDrinks;

    int treshold = 6;
};

class PaninoBuca : public PizzaCompanyFactory {
public:
    Pizza *CreatePizza() {
        cout << "Veganpizza is coming!!!" << endl;
        return new VeganPizza();
    }

    Drink *CreateDrink() {
        cout << "Coke is comingggg!!!" << endl;
        return new CokeDrink();
    }

    void addNonVeganPizza(Pizza *pizza) {
        cout << "Adding pizza " << _veganPizzas.size() << endl;
        _veganPizzas.push_back(pizza);
    }

    void addSodaDrink(Drink *drink) {
        _sodaDrinks.push_back(drink);
    }

    void consumePizza() {
        if (_veganPizzas.size() < treshold) {
            SendReport();
            cout << "Sending Report" << endl;
            return;
        }

        _veganPizzas.erase(_veganPizzas.begin());
        cout << "Consuming " << _veganPizzas.size() << endl;
    }

    string GetCompanyName() {
        return "Panino Buca";
    }

    int GetDistance() {
        return 18;
    }

private:
    vector<Pizza *> _veganPizzas;
    vector<Drink *> _sodaDrinks;

    int treshold = 4;
};

// Client
class OrderCreator {
public:
    void CreatePanino(PizzaCompanyFactory *factory) {
        _pizza = factory->CreatePizza();
        _drink = factory->CreateDrink();
    }

    Pizza *GetPizza() const {
        return _pizza;
    }

    Drink *GetDrink() const {
        return _drink;
    }

private:
    Pizza *_pizza;
    Drink *_drink;
};

int main() {
    DistributorCenter *s = DistributorCenter::getInstance("Ahmet");

    PizzaCompanyFactory *panino = new PaninoBalcova();
    OrderCreator o;
    o.CreatePanino(panino);

    PaninoBalcova *p = new PaninoBalcova();
    p->Attach(s);

    for (int i = 1; i <= 10; ++i) {
        p->AddVeganPizza(o.GetPizza());
    }

    for (int j = 1; j <= 10; ++j) {
        p->ConsumePizza();
    }

    p->Detach(s);

    delete panino;
    delete p;
    delete s;
}