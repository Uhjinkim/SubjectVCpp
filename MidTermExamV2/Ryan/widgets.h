#pragma once

struct MARGIN {
    int left = 0;
    int top = 0;
    int right = 0;
    int bottom = 0;

    void setMargin(int marginAll) {
        left = marginAll; 
        top = marginAll;
        right = marginAll;
        bottom = marginAll;
    }
    void setMargin(int marginHorizental, int marginVertical) {
        left = marginHorizental;
        top = marginVertical;
        right = marginHorizental;
        bottom = marginVertical;
    }
    void setMargin(int marginLeft, int marginTop, int marginRight, int marginBottom) {
        left = marginLeft;
        top = marginTop;
        right = marginRight + marginLeft;
        bottom = marginBottom + marginTop;
    }
};

struct PADDING {
    int left = 0;
    int top = 0;
    int right = 0;
    int bottom = 0;
    RECT padding = { left, top, right, bottom };

    void setPadding(int paddingAll) {
        if (paddingAll > 0) {
            left = paddingAll;
            top = paddingAll;
            right = paddingAll;
            bottom = paddingAll;
        }
    }
    void setPadding(int paddingHorizental, int paddingVertical) {
        if (paddingHorizental > 0 && paddingVertical > 0) {
            left = paddingHorizental;
            top = paddingVertical;
            right = paddingHorizental;
            bottom = paddingVertical;
        }
    }
    void setPadding(int paddingLeft, int paddingTop, int paddingRight, int paddingBottom) {
        left = paddingLeft;
        top = paddingTop;
        right = paddingRight;
        bottom = paddingBottom;
    }
};

struct Widget {
private:
protected:
    wchar_t* windowName;
    wchar_t* className;
public:
    Widget* parent;
    DWORD dwExStyle = 0;
    DWORD dwStyle = 0;
    MARGIN margin;
    PADDING padding;
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    HMENU hMenu = nullptr;
    Widget() {
        windowName = nullptr;
        className = nullptr;
        setParent(nullptr);

    }
    ~Widget() {
        delete[] windowName;
        delete[] className;
    }

    wchar_t* getWindowName() {
        if (windowName != nullptr)
            return windowName;
        else return nullptr;
    }

    void setWindowName(const wchar_t* name) {
        delete[] windowName;
        size_t length = wcslen(name) + 1;
        windowName = new wchar_t[length];
        wcscpy_s(windowName, length, name);
    }

    wchar_t* getClassName() {
        if (className != nullptr)
            return className;
        else return nullptr;
    }

    void setClassName(const wchar_t* name) {
        delete[] className;
        size_t length = wcslen(name) + 1;
        className = new wchar_t[length];
        wcscpy_s(className, length, name);
    }

    void setParent(Widget* _parent) {
        parent = _parent;
        if (parent != nullptr) {
            setStartPoint(0, 0);
            setWidthAuto();
            setHeightAuto();
        }
    }
    void setStartPoint(int _x, int _y) {
        x = _x + parent->padding.left + margin.left;
        y = _y + parent->padding.top + margin.top;
    }
    void setWidthAuto() {
        width = parent->width - parent->padding.right - margin.right - x;
    }
    void setWidthAuto(int _width) {
        width = _width - parent->padding.right - margin.right - x;
    }
    void setWidth(int _width) {
        width = _width;
    }
    void setHeightAuto() {
        height = parent->height - parent->padding.bottom - margin.bottom - y;
    }
    void setHeightAuto(int _height) {
        height = _height - parent->padding.bottom - margin.bottom - y;
    }
    void setHeight(int _height) {
        height = _height;
    }
};

struct ButtonWidget : Widget {
    ButtonWidget() : Widget() {
        dwStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
        margin.setMargin(16);
        padding.setPadding(0);
    }
    ~ButtonWidget() {
    }
};
