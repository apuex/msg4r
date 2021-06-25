#include <iostream>
#include <memory>

enum class animal_type {
  CAT = 0,
  DOG = 1
};

class animal {
public:
  virtual ~animal() {}
  virtual void action() = 0;
protected:
  animal(animal_type type) : type_(type) {}
  const animal_type type_;
};

class cat: public animal {
public:
  cat() : animal(animal_type::CAT), name_("tom") {}
  cat(const std::string& name) : animal(animal_type::CAT), name_(name) {}
  virtual ~cat() {}
  void action() { std::cout << name_ << "\t: mew!" << std::endl; }
  const std::string name_;
};

class dog: public animal {
public:
  dog() : animal(animal_type::DOG), name_("spike") {}
  dog(const std::string& name) : animal(animal_type::DOG), name_(name) {}
  virtual ~dog() {}
  void action() { std::cout << name_ << "\t: bark!" << std::endl; }
  const std::string name_;
};

int main(int argc, char* argv[]) {
  std::shared_ptr<animal> null_animal;
  std::shared_ptr<animal> pussy_cat = std::make_shared<cat>("Thomas");
  std::shared_ptr<animal> friendly_dog = std::make_shared<dog>("Fido");

  std::cout << "[PLAY 1, Opening, synopsis..]" << std::endl;
  pussy_cat->action();
  friendly_dog->action();
  std::cout << "[PLAY 2, Swap soul, synopsis..]" << std::endl;
  pussy_cat.swap(friendly_dog);
  null_animal = friendly_dog;
  pussy_cat->action();
  friendly_dog->action();
  null_animal->action();
	return 0;
}
