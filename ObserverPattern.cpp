

/*# Observer-Pattern-MVC-
Observer Pattern Implementation : Lesson

The observer pattern file, how it can be used to create an MVC designed pattern

This pattern is the main reason MVC works and its used in the model gui frame work for listening for events.
Device manufacture also used the pattern to listen to a sensor for realtime application.
This is pattern is sometimes used with the state pattern to keep track of the subject state.


What I did you is to twist and for sending signal from subject to observer.


*/
#include <iostream>
#include<set>
using namespace std;
class Observer;
class Subject;
class Signal;
class Observer{
 public:
    virtual void  notify(Signal *signal)=0;
};


class Signal{
private:
    unsigned int msg;
    Subject *__subject;
public:
    Signal(Subject *subject, unsigned int msg){
      __subject = subject;
       this->msg=msg;
    };
    unsigned int getMessage(){return msg;}
    Subject *subject(){return __subject;}

};


class Subject:public Observer{
  private:
     std::set<Observer*> observers;

  protected:
    Subject(){};
  public:

      void attach(Observer *observer){
           if(observer !=0)
                observers.insert(observer);
      }
      void detach(Observer *observer){
        std::set<Observer*>::iterator iter= this->observers.begin();
          for(iter; iter != this->observers.end(); iter++)
          {
             Observer *obsr = *iter;
             if(obsr == observer){
               this->observers.erase(iter);
               break;
             }

          }
      }
  virtual void  notify(Signal *signal){
      std::set<Observer*>::iterator iter= this->observers.begin();
      for(iter; iter != this->observers.end(); iter++)
      {
         Observer *observer = *iter;
         observer->notify(signal);
      }
   }
};


/*
  CLICK =0;
  MOVE = 1;

*/

 class ButtonModel:public Subject{

  int state;
   public:
       ButtonModel(int s){state=0;}
       int getState(){return state;}
       void setState(int s){state=s;
         this->notify(new Signal(this,0));
       }

 };
class JButton:public Subject{
public:
    JButton(){}
    void click(){
        Signal *signal = new Signal(this,0);
        this->notify(signal);
    };
    void render(ButtonModel *model){
      std::cout<<"state change = "<< model->getState()<<"\n";
    }


 };


 class ButtonEventListener:public Observer{
 public:
     ButtonEventListener(){}
     virtual void  notify(Signal *signal){
         if(signal->getMessage()==0)
         {
          this->onClickEvent(signal->subject());

         }else{
             this->onMoveEvent(signal->subject());
         }
     };

     virtual void onMoveEvent(Subject *subject)=0;
     virtual void onClickEvent(Subject * subject)=0;

 };

class ImpButtonEvent:public ButtonEventListener
{
   ButtonModel *model;
   JButton     *view;

  public:
       ImpButtonEvent(JButton *button, ButtonModel *model)
       {
                this->model=model;
                this->view =button;
       }
     virtual void onMoveEvent(Subject *subject){

         if(subject==model)
         {
             view->render(model);
         }else if(subject==view){
             model->setState(0);
         }
     };
     virtual void onClickEvent(Subject * subject){
        if(subject==view){
             model->setState(1);
         }else if(subject==model)
            view->render(model);
     };

};

int main()
{
    ButtonModel *model= new ButtonModel(0);
    JButton *view= new JButton();

    ButtonEventListener *listener= new ImpButtonEvent(view,model);
    view->attach(listener);
    model->attach(listener);
    model->setState(56);
    model->setState(100);
    view->click();
    model->setState(21100);
    view->click();

    return 0;
}
