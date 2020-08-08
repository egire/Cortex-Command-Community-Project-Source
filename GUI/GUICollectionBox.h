#ifndef _GUICOLLECTIONBOX_
#define _GUICOLLECTIONBOX_

//////////////////////////////////////////////////////////////////////////////////////////
// File:            GUICollectionBox.h
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     GUICollectionBox class
// Project:         GUI Library
// Author(s):       Jason Boettcher
//                  jackal@shplorb.com
//                  www.shplorb.com/~jackal


namespace RTE
{


//////////////////////////////////////////////////////////////////////////////////////////
// Class:           GUICollectionBox
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     A collection box control class that contains child controls.
// Parent(s):       GUIControl, Panel.
// Class history:   1/15/2004 GUICollectionBox Created.

class GUICollectionBox :
    public GUIControl,
    public GUIPanel
{

//////////////////////////////////////////////////////////////////////////////////////////
// Public member variable, method and friend function declarations

public:

    // CollectionBox Notifications
    enum {
        Clicked = 0,
        MouseMove    // Mouse moved over the panel
    } Notification;

    // Drawing type
    enum {
        Color,
        Image,
        Panel
    } DrawType;


//////////////////////////////////////////////////////////////////////////////////////////
// Constructor:     GUICollectionBox
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Constructor method used to instantiate a GUICollectionBox object in
//                  system memory.
// Arguments:       GUIManager, GUIControlManager.

    GUICollectionBox(GUIManager *Manager, GUIControlManager *ControlManager);


//////////////////////////////////////////////////////////////////////////////////////////
// Destructor:      ~GUICollectionBox
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Destructor method used to clean up this before deletion from memory.
// Arguments:       None.

	~GUICollectionBox() { Destroy(); }


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
// Virtual method:  Destroy
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Destroys and frees this' allocated data
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
// Method:  OnMouseDown
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the mouse goes down on the panel
// Arguments:       Mouse Position, Mouse Buttons, Modifier.

    void OnMouseDown(int X, int Y, int Buttons, int Modifier) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:  OnMouseUp
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the mouse goes up on the panel
// Arguments:       Mouse Position, Mouse Buttons, Modifier.

    void OnMouseUp(int X, int Y, int Buttons, int Modifier) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          OnMouseMove
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the mouse moves (over the panel, or when captured).
// Arguments:       Mouse Position, Mouse Buttons, Modifier.

    void OnMouseMove(int X, int Y, int Buttons, int Modifier) override;


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

    static std::string GetControlID()    { return "COLLECTIONBOX"; };


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          StoreProperties
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Gets the control to store the values into properties.
// Arguments:       None.

    void StoreProperties() override;


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          SetDrawImage
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Sets the drawing image bitmap to draw
// Arguments:       Bitmap, ownership IS transferred!

    void SetDrawImage(GUIBitmap *Bitmap);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetDrawImage
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Gets the drawing image bitmap that is being drawn
// Arguments:       Bitmap, ownership IS NOT transferred!

    GUIBitmap * GetDrawImage() { return m_DrawBitmap; }


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          SetDrawBackground
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Sets whether to draw the background.
// Arguments:       Draw.

    void SetDrawBackground(bool DrawBack);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          SetDrawType
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Sets the drawing type.
// Arguments:       Type.

    void SetDrawType(int Type);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetDrawType
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Gets the current drawing type.
// Arguments:       None.
// Returns:         Type.

    int GetDrawType() const { return m_DrawType; }


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          SetDrawColor
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Sets the drawing color.
// Arguments:       Color.

    void SetDrawColor(unsigned long Color);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetDrawColor
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Gets the drawing color.
// Returns:         Color.

    unsigned long GetDrawColor() const { return m_DrawColor; }


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          ApplyProperties
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Applies new properties to the control.
// Arguments:       GUIProperties.

    void ApplyProperties(GUIProperties *Props) override;


//////////////////////////////////////////////////////////////////////////////////////////
// Private member variable and method declarations

private:


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          BuildBitmap
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Create the button bitmap to draw.
// Arguments:       None.

    void BuildBitmap();


// Member data

    GUIBitmap        *m_Background;

    bool            m_DrawBackground;
    int                m_DrawType;
    unsigned long            m_DrawColor;
    GUIBitmap        *m_DrawBitmap;


};


}; // namespace RTE


#endif  //  _GUICOLLECTIONBOX_