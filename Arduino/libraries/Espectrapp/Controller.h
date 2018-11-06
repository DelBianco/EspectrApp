/*
  Colors.h - [description]
*/
#include "Arduino.h"
#include "View.h"
#include "Map.h"

#ifndef Controller_h
#define Controller_h

class Controller{
  public:
    Controller();
    void startAction(int,int,int,int);
    void paramsAction(int);
    void menuAction();
    void graphAction(int *,int);
    void transAction();
    void absorAction();
    void baseAction();
    void realAction();
    void storageAction();
    void savesdAction();
    void dataListAction(int,int,int);
    void dataViewAction();
    void calcMax(int *,int);
    void dataChanged();
    void getTouchScreen();
    void sendPhoton(String);
    String mapActions();
    String getRoute();
    void setRoute(String);
    void setNumLog(int);
    bool hasBase();
  private:
    View * _view;
    Map * _map;
    int _dataStash[145];
    String _currentRoute;
    bool _dataChanged = false;
    String _route;
    int _max = 0;
    int _numLogs = 0;
    int _min = 0;
    int _tInt = 100;
    bool _base = false;
    int _s1,_s2,_s3,_s4;
};

#endif
