#include<bits/stdc++.h>
#include<string>


// button

class IButton {
public:
    virtual std::string Operation() const = 0;
    virtual ~IButton() {}
};


class WindowsButton: public IButton{
    std::string Operation() const override{
        return "Windows button\n";
    }
};

class MacButton: public IButton{
    std::string Operation() const override {
        return "Mac button\n";
    }
};


class IDialog{
   public:
   virtual ~IDialog(){};
   virtual IButton* FactoryMethod() const = 0;
   std::string printDetails() const {
    IButton* button = this->FactoryMethod();
    std::string op = "This is a "+button->Operation();

    delete button;
    return op;
   }
};


class WindowsDialog : public IDialog {
    public:
    IButton* FactoryMethod() const override{
        return new WindowsButton();
    }
};

class MacDialog : public IDialog {
    public:
    IButton* FactoryMethod() const override{
        return new MacButton();
    }
};


void ClientCode(const IDialog& dialog){
    std::cout<<"client calling "<<dialog.printDetails();
}

int main(){
   IDialog* wd = new WindowsDialog();
   ClientCode(*wd);
   delete wd;
   IDialog* md = new MacDialog();
   ClientCode(*md);
   delete md;
}