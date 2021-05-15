#ifndef _GUITEXTBOX_
#define _GUITEXTBOX_

//////////////////////////////////////////////////////////////////////////////////////////
// File:            GUITextBox.h
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     GUITextBox class
// Project:         GUI Library
// Author(s):       Jason Boettcher
//                  jackal@shplorb.com
//                  www.shplorb.com/~jackal


//////////////////////////////////////////////////////////////////////////////////////////
// Inclusions of header files

#include "GUITextPanel.h"


namespace RTE
{


//////////////////////////////////////////////////////////////////////////////////////////
// Class:           GUITextBox
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     A TextBox control class.
// Parent(s):       GUIControl, GUITextPanel.
// Class history:   1/20/2004 GUITextBox Created.

class GUITextBox :
    public GUIControl,
    public GUITextPanel
{

//////////////////////////////////////////////////////////////////////////////////////////
// Public member variable, method and friend function declarations

public:

    // Notifications
    enum {
        Changed = 0,
        Clicked,
        Enter
    } Notification;


//////////////////////////////////////////////////////////////////////////////////////////
// Constructor:     GUITextBox
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Constructor method used to instantiate a GUITextBox object in
//                  system memory.
// Arguments:       GUIManager, GUIControlManager.

    GUITextBox(GUIManager *Manager, GUIControlManager *ControlManager);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Create
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the control has been created.
// Arguments:       Name, Position.

    void Create(const std::string Name, int X, int Y, int Width = -1, int Height = -1) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Create
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the control has been created.
// Arguments:       Properties.

    void Create(GUIProperties *Props) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Destroy
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the control has been destroyed.
// Arguments:       None.

    void Destroy() override; 


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          ChangeSkin
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the skin has been changed.
// Arguments:       New skin pointer.

    void ChangeSkin(GUISkin *Skin) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Draw
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Draws the panel
// Arguments:       Screen class

    void Draw(GUIScreen *Screen) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetPanel
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Returns the panel of the control.
// Arguments:       None.
// Returns:         0 if the control does not have a panel, otherwise the topmost panel.

    GUIPanel * GetPanel() override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetControlID
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Returns a string representing the control's ID
// Arguments:       None.

    static std::string GetControlID()    { return "TEXTBOX"; };


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Move
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the control needs to be moved.
// Arguments:       New position.

    void Move(int X, int Y) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Resize
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the control needs to be resized.
// Arguments:       New size.

    void Resize(int Width, int Height) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetControlRect
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Gets the rectangle of the control.
// Arguments:       Position, Size.

    void GetControlRect(int *X, int *Y, int *Width, int *Height) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          ReceiveSignal
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when receiving a signal.
// Arguments:       Signal source, Signal code, Signal data.

    void ReceiveSignal(GUIPanel *Source, int Code, int Data) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          ApplyProperties
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Applies new properties to the control.
// Arguments:       GUIProperties.

    void ApplyProperties(GUIProperties *Props) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Private member variable and method declarations

private:

    GUIBitmap        *m_DrawBitmap;
    int              m_HAlignment;
    int              m_VAlignment;

};


}; // namespace RTE


#endif  //  _GUITEXTBOX_