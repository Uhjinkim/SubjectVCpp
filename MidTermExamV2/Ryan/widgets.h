#pragma once

typedef struct MARGIN {
    int left = 0;
    int top = 0;
    int right = 0;
    int bottom = 0;

    void setMargin(int marginAll) {
        left = marginAll; 
        top = marginAll;
        right = marginAll * 2;
        bottom = marginAll * 2;
    }
    void setMargin(int marginHorizental, int marginVertical) {
        left = marginHorizental;
        top = marginVertical;
        right = marginHorizental * 2;
        bottom = marginVertical * 2;
    }
    void setMargin(int marginLeft, int marginTop, int marginRight, int marginBottom) {
        left = marginLeft;
        top = marginTop;
        right = marginRight + marginLeft;
        bottom = marginBottom + marginTop;
    }
};

typedef struct PADDING {
    int left = 0;
    int top = 0;
    int right = 0;
    int bottom = 0;
    RECT padding = { left, top, right, bottom };

    void setPadding(int paddingAll) {
        if (paddingAll > 0) {
            left = paddingAll;
            top = paddingAll;
            right = paddingAll * 2;
            bottom = paddingAll * 2;
        }
    }
    void setPadding(int paddingHorizental, int paddingVertical) {
        if (paddingHorizental > 0 && paddingVertical > 0) {
            left = paddingHorizental;
            top = paddingVertical;
            right = paddingHorizental * 2;
            bottom = paddingVertical * 2;
        }
    }
    void setPadding(int paddingLeft, int paddingTop, int paddingRight, int paddingBottom) {
        left = paddingLeft;
        top = paddingTop;
        right = paddingRight + paddingLeft;
        bottom = paddingBottom + paddingTop;
    }
};

struct Widget {
private:
protected:
    wchar_t* windowName;
    wchar_t* className;
public:
    DWORD dwExStyle = 0;
    DWORD dwStyle = {};
    MARGIN margin;
    PADDING padding;
    int width = 0;
    int height = 0;
    HMENU hMenu = nullptr;
    Widget() {
        windowName = nullptr;
        className = nullptr;

    }
    ~Widget() {
        delete[] windowName;
        delete[] className;
    }

    /*
    void setMargin(int margininput) {
        margin = { margininput, margininput, margininput*2, margininput*2 };
    }
    void setMargin(int marginHorizental, int marginVertical) {
        margin = { marginHorizental, marginVertical, marginHorizental*2, marginVertical*2 };
    }
    void setMargin(int marginLeft, int marginTop, int marginRight, int marginBottom) {
        margin = { marginLeft, marginTop, marginRight+marginLeft, marginBottom+marginTop };
    }
    */

    wchar_t* getWindowName() {
        if (windowName != nullptr)
            return windowName;
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
    }

    void setClassName(const wchar_t* name) {
        delete[] className;
        size_t length = wcslen(name);
        className = new wchar_t[length];
        wcscpy_s(className, length, name);
    }
};




struct ButtonWidget : Widget {
    int menu= 0;
    ButtonWidget() : Widget() {
        setClassName(L"BUTTON");
        dwStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
        margin.setMargin(16);
    }
    ~ButtonWidget() {
        delete[] windowName;
        delete[] className;
    }
};
