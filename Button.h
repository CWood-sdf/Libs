#pragma once
#define BRAIN_WIDTH 480
#define BRAIN_HEIGHT 220
#include <functional>
#include <string>
namespace vex
{
    class brain;
    class color;
}
struct Button
{
protected:
    brain& Brain;
    int x;
    int y;
    int width;
    int height;
    string label;
    color fill = color(0, 0, 0);
    color fill2 = color(50, 50, 50);
    int xOff = 0;
    int yOff = 0;
    bool pressedLast = false;

public:
    explicit Button(brain& Brain, int x, int y, int width, int height, color fill, string label)
      : Brain(Brain),
        x(x),
        y(y),
        width(width),
        height(height),
        label(label)
    {
        this->fill = fill;
        fill2 = fill; // sdfssd
    }
    explicit Button(brain& Brain, int x, int y, int width, int height, color fill, string label, int xOff, int yOff)
      : Brain(Brain),
        x(x),
        y(y),
        width(width),
        height(height),
        label(label),
        xOff(xOff),
        yOff(yOff)
    {
        this->fill = fill;
        fill2 = fill;
    }
    explicit Button(brain& Brain, int x, int y, int width, int height, color fill, color fill2, string label, int xOff = 0, int yOff = 0)
      : Brain(Brain),
        x(x),
        y(y),
        width(width),
        height(height),
        label(label),
        xOff(xOff),
        yOff(yOff)
    {
        this->fill = fill;
        this->fill2 = fill2;
    }
    explicit Button(Button& b, int x, int y) : Button(b)
    {
        this->x = x;
        this->y = y;
    }
    void draw()
    {
        // fill(this.fill);
        Brain.Screen.setFillColor(pressing() ? this->fill2 : this->fill);
        Brain.Screen.drawRectangle(x, y, width, height);

        Brain.Screen.setCursor(2, 1);
        Brain.Screen.printAt(x + 50 + xOff, y + 50 + yOff, this->label.data());
    }
    void drawClick()
    {
        // fill(this.fill);
        Brain.Screen.setFillColor(this->fill2);
        Brain.Screen.drawRectangle(x, y, width, height);

        Brain.Screen.setCursor(2, 1);
        Brain.Screen.printAt(x + 50 + xOff, y + 50 + yOff, this->label.data());
    }
    bool pressing()
    {
        int mouseX = Brain.Screen.xPosition();
        int mouseY = Brain.Screen.yPosition();
        if (Brain.Screen.pressing() && mouseX > x &&
            mouseX < x + width &&
            mouseY > y &&
            mouseY < y + height)
        {

            return true;
        }
        return false;
    }
    bool clicked()
    {
        if (pressing())
        {

            while (Brain.Screen.pressing())
            {
                drawClick();
                task::sleep(10);
            }

            return true;
        }
        return false;
    }
    bool released()
    {
        // Return true when pressing() is false but lastPressed is true
        bool p = pressing();
        if (!p && !Brain.Screen.pressing() && pressedLast)
        {
            pressedLast = false;
            return true;
        }
        pressedLast = p;
        return false;
    }
    bool pressed()
    {
        // Return true when pressing() is true but lastPressed is false
        bool p = pressing();
        if (p && !pressedLast)
        {
            pressedLast = true;
            return true;
        }
        pressedLast = p;
        return false;
    }
};
struct Button2
{
    brain& Brain;
    int x;
    int y;
    int width;
    int height;
    void (*function)(void);
    const char* label;
    const color& fill;
    const color* fill2;
    int xOff = 0;
    int yOff = 0;
    string& what;
    string toWhat;
    explicit Button2(brain& Brain, int x, int y, int width, int height, string& what, string toWhat, const color& fill, const char* label)
      : Brain(Brain),
        x(x),
        y(y),
        width(width),
        height(height),
        label(label),
        fill(fill),
        what(what),
        toWhat(toWhat)
    {
        fill2 = &fill;
    }
    explicit Button2(brain& Brain, int x, int y, int width, int height, string& what, string toWhat, const color& fill, const char* label, int xOff, int yOff)
      : Brain(Brain),
        x(x),
        y(y),
        width(width),
        height(height),
        label(label),
        fill(fill),
        xOff(xOff),
        yOff(yOff),
        what(what),
        toWhat(toWhat)
    {
        fill2 = &fill;
    }
    explicit Button2(brain& Brain, int x, int y, int width, int height, string& what, string toWhat, const color& fill, const color& fill2, const char* label, int xOff, int yOff)
      : Brain(Brain),
        x(x),
        y(y),
        width(width),
        height(height),
        label(label),
        fill(fill),
        fill2(&fill2),
        xOff(xOff),
        yOff(yOff),
        what(what),
        toWhat(toWhat)
    {
    }
    /*explicit Button2(Button& b, int x, int y) : Button(b){
        this->x = x;
        this->y = y;

    }*/
    void draw(const char* label = "")
    {
        // fill(this.fill);
        Brain.Screen.setFillColor(this->fill);
        Brain.Screen.drawRectangle(x, y, width, height);

        Brain.Screen.setCursor(2, 1);
        Brain.Screen.printAt(x + 50 + xOff, y + 50 + yOff, this->label);
    }
    void drawClick()
    {
        Brain.Screen.setFillColor(*this->fill2);
        Brain.Screen.drawRectangle(x, y, width, height);

        Brain.Screen.setCursor(2, 1);
        Brain.Screen.printAt(x + 50 + xOff, y + 50 + yOff, this->label);
    }
    void click()
    {
        if (clicked())
            what = toWhat;
    }
    bool pressed()
    {
        int mouseX = Brain.Screen.xPosition();
        int mouseY = Brain.Screen.yPosition();
        return Brain.Screen.pressing() &&
               mouseX > x &&
               mouseX < x + width &&
               mouseY > y &&
               mouseY < y + height;
    }
    bool clicked()
    {
        if (pressed())
        {
            while (pressed())
            {
                drawClick();
                task::sleep(1);
            }
            return true;
        }
        return false;
    }
};
struct MovingButton : public Button
{
private:
    bool clickedLast = false;

public:
    explicit MovingButton(brain& Brain, int x, int y, int width, int height, const color& fill, string label)
      : Button(Brain, x, y, width, height, fill, label)
    {
    }
    explicit MovingButton(brain& Brain, int x, int y, int width, int height, const color& fill, string label, int xOff, int yOff)
      : Button(Brain, x, y, width, height, fill, label, xOff, yOff)
    {
    }
    explicit MovingButton(brain& Brain, int x, int y, int width, int height, const color& fill, color fill2, string label, int xOff, int yOff)
      : Button(Brain, x, y, width, height, fill, fill2, label, xOff, yOff)
    {
    }
    explicit MovingButton(Button& b, int x, int y) : Button(b, x, y)
    {
    }
    void draw(const char* label = "")
    {
        // fill(this.fill);
        bool p = pressing();
        Brain.Screen.setFillColor(p ? this->fill2 : this->fill);
        Brain.Screen.drawRectangle(x, y, width, height);

        Brain.Screen.setCursor(2, 1);
        Brain.Screen.printAt(x + 50 + xOff, y + 50 + yOff, this->label.data());
        if (p || (clickedLast && Brain.Screen.pressing()))
        {
            x = Brain.Screen.xPosition();
            y = Brain.Screen.yPosition();
        }
        clickedLast = p;
    }
};
struct Scroll : public Button
{
    int mouseX = 0;
    int maxX = 0;
    explicit Scroll(brain& Brain, int x, int y, int width, int height, const color& fill, string label)
      : Button(Brain, x, y, width, height, fill, label)
    {
        maxX = width;
    }
    explicit Scroll(brain& Brain, int x, int y, int width, int height, const color& fill, string label, int xOff, int yOff)
      : Button(Brain, x, y, width, height, fill, label, xOff, yOff)
    {
        maxX = width;
    }
    explicit Scroll(Scroll& s, int x, int y) : Scroll(s)
    {
        this->x = x;
        this->y = y;
    }
    bool clicked()
    {
        int mouseX = Brain.Screen.xPosition();
        int mouseY = Brain.Screen.yPosition();
        return Brain.Screen.pressing() &&
               mouseX > x &&
               mouseX < x + width &&
               mouseY > y &&
               mouseY < y + height;
    }
    void draw(const char* label = "")
    {
        // fill(this.fill);
        Brain.Screen.setFillColor(fill);
        Brain.Screen.drawRectangle(x, y, width, height);
        adjustPos();
        Brain.Screen.setFillColor(black);
        Brain.Screen.drawRectangle(x + mouseX, y, 10, height);
        Brain.Screen.setFillColor(fill);
        Brain.Screen.setCursor(2, 1);
        Brain.Screen.printAt(x + 50 + xOff, y + 50 + yOff, this->label.data());
    }
    int adjustPos()
    {
        if (clicked())
        {
            mouseX = Brain.Screen.xPosition() - x;
        }
        return mouseX;
    }
};