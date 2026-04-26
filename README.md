# tsekI

## Key Structs and Enums

### tsekIWindow
A handle to a window. Contains a pointer to the platform-specific data for a certain window. Do not attempt to access this yourself, use defined platform-generic functions to extract and modify data.

### tsekIContext
A handle to a context. Contains a pointer to the platform-specific data for a global context. Again, do not attempt to access this yourself, use defined platform-generic functions for this purpose. One tsekIContext is bound at a time - this will soon be changed to be thread safe.

### tsekKeyCode
An enum containing a member for each key on a keyboard - redefined as to be platform agnostic. It includes toggles and mouse buttons.

### tsekIWindowParam
An enum containing parameters that can be set or retrieved for a specific window. More details in the `tsekI_get_parameter` and `tsekI_set_parameter` sections.

### tsekPixelFormat
A struct to contain a preference for the data stored in each pixel of the screen. Note that data "suggested" in this way is not set, but is instead used to rank available pixel formats and choose the best. The members are:  

`uint16_t r_bits, g_bits, b_bits, a_bits`
Set the number of bits in each of the 4 respective channels. Defaults to 8.  

`uint16_t depth_bits`
Sets the number of bits used in storing depth information. Defaults to 24.  

`uint16_t stencil_bits`
Sets the number of bits used in the stencil buffer. Defaults to 8.  

`uint8_t samples`
Sets the numbers of samples per pixel. Defaults to 4.  

### tsekIWindowInfo
A struct to contain data used in window initialisation. Contains

`const wchar_t* title`
The title of the window. Only used in Windows.

`uint32_t width, height`
The dimensions of the window. Overwritten by most Linux WMs.

`uint32_t classId`, `const wchar_t* wndClassName`
The windows class specifiers. Usually redundant, can be ignored.

`uint32_t style, extendedStyle`
Used in defining windows style in certain window managers.

`tsekPixelFormat pixelFormat`
The preferred pixel format.

### tsekCallbacks
Defines functions to be called when certain things change about a window. They are:

`void (*keydown)(tsekIWindow*, tsekKeyCode);`
Called on keydown.

`void (*keytype)(tsekIWindow*, tsekKeyCode);`
Called on keytype; the repeated call when a key is held down.

`void (*keyup)(tsekIWindow*, tsekKeyCode);`
Called on keyup.

`void (*mbdown)(tsekIWindow*, tsekKeyCode);`
Called on mouse button down.

`void (*mbup)(tsekIWindow*, tsekKeyCode);`
Called on mouse button up.

`void (*kstate)(tsekIWindow*, tsekKeyCode, bool);`
Called on key state (caps lock, num lock, scroll lock) toggle.

`void (*size)(tsekIWindow*, uint32_t width, uint32_t height);`
Called on window resize.

`void (*tsegsize)(tsekIWindow*, uint32_t width, uint32_t height);`
Called on window resize, reserved for graphics APIs.

## Functions
