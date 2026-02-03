#ifndef __LCD__
#define __LCD__

class Lcd {
public:
  virtual void print(const String& msg) = 0;
  virtual void setCursor(int col, int row) = 0;
  virtual void clear() = 0;
};

#endif