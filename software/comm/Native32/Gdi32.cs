using System;
using System.Collections.Generic;
using System.Text;
using System.Security;
using System.Runtime.InteropServices;
using System.Drawing;

namespace Native32
{
    [SuppressUnmanagedCodeSecurity]
    class Gdi32
    {

        [DllImport("user32.dll")]
        public static extern IntPtr GetDC(
            IntPtr hWnd);
        [DllImport("user32.dll")]
        public static extern int ReleaseDC(
            IntPtr hWnd,
            IntPtr hDC);

        [DllImport("gdi32.dll", ExactSpelling = true, PreserveSig = true, SetLastError = true)]
        public static extern IntPtr SelectObject(
            IntPtr hDC,
            IntPtr hGDIObject);
        [DllImport("gdi32.dll")]
        public static extern bool DeleteObject(
            IntPtr hGDIObject);



        [DllImport("gdi32.dll")]
        public static extern int GetDeviceCaps(
            IntPtr hDC,
            DeviceCap Index);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="nHeight">The height of the font's character cell, in logical units (also known as the em height). If positive, the font mapper converts this value directly into device units and matches it with the cell height of the possible fonts. If 0, the font mapper uses a default character height. If negative, the font mapper converts the absolute value into device units and matches it with the character height of the possible fonts. </param>
        /// <param name="nWidth">The average width of the font's characters. If 0, the font mapper tries to determine the best value. </param>
        /// <param name="nEscapement">The angle between the font's baseline and escapement vectors, in units of 1/10 degrees. Windows 95, 98: This must be equal to lfOrientation. </param>
        /// <param name="nOrientation">The angle between the font's baseline and the device's x-axis, in units of 1/10 degrees. Windows 95, 98: This must be equal to lfEscapement. </param>
        /// <param name="fnWeight">Flag specifying the boldness (weight) of the font</param>
        /// <param name="fdwItalic">A non-zero value if the font is italicized, 0 if not.</param>
        /// <param name="fdwUnderline">A non-zero value if the font is underlined, 0 if not. </param>
        /// <param name="fdwStrikeOut">A non-zero value if the font is striked out, 0 if not.</param>
        /// <param name="fdwCharSet">Exactly one of the following flags specifying the character set of the font: </param>
        /// <param name="fdwOutputPrecision">Exactly one of the following flags specifying the desired precision (closeness of the match) between the logical font ideally described by the structure and the actual logical font. This value is used by the font mapper to produce the logical font. </param>
        /// <param name="fdwClipPrecision">Exactly one of the following flags specifying the clipping precision to use when the font's characters must be clipped: </param>
        /// <param name="fdwQuality">Exactly one of the following flags specifying the output quality of the logical font as compared to the ideal font:</param>
        /// <param name="fdwPitchAndFamily">A bitwise OR combination of exactly one *_PITCH flag specifying the pitch of the font and exactly one FF_* flag specifying the font face family of the font: </param>
        /// <param name="lpszFace">The name of the font face to use. </param>
        /// <returns>Uchwyt do obiektu GDI</returns>
        [DllImport("gdi32.dll")]
        public extern static IntPtr CreateFont(
            int nHeight,
            int nWidth,
            int nEscapement,
            int nOrientation,
            FontWeight fnWeight,
            FontItalic fdwItalic,
            FontUnderline fdwUnderline,
            FontStrikeOut fdwStrikeOut,
            FontCharset fdwCharSet,
            OutputPrecision fdwOutputPrecision,
            FontClipPrecision fdwClipPrecision,
            FontQuality fdwQuality,
            FontPitchAndFamily fdwPitchAndFamily,
            string lpszFace);


        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern int MulDiv(
            int nNumber,
            int nNumerator,
            int nDenominator);

        [DllImport("gdi32", EntryPoint = "TextOut")]
        public static extern int TextOut(
            IntPtr hDC,
            int x,
            int y,
            [In] string lpString,
            int nCount);

        [DllImport("gdi32.dll", CharSet = CharSet.Auto)]
        public static extern bool GetTextMetrics(
            IntPtr hDC,
            [Out] TEXTMETRIC tm);

        [DllImport("gdi32.dll")]
        public static extern int SetTextColor(
            IntPtr hdc,
            int crColor);

        [DllImport("gdi32.dll")]
        public static extern int SetBkColor(
            IntPtr hdc,
            int crColor);




        [DllImport("gdi32.dll", CharSet = CharSet.Unicode, SetLastError = true, EntryPoint = "GetTextExtentPoint32W")]
        public static extern bool GetTextExtentPoint(
            [In]IntPtr hdc,
            [In]string lpString,
            [In]int cbString,
            [Out] out SIZE lpSize);


        [DllImport("gdi32.dll")]
        public static extern bool GetCharWidth32(
            IntPtr hdc,
            uint iFirstChar,
            uint iLastChar,
            [Out] int[] lpBuffer);

        [DllImport("gdi32.dll")]
        public static extern bool GetCharWidthI(
            IntPtr hdc,
            uint giFirst,
            uint cgi,
            [In] ushort[] pgi,
            [Out] int[] lpBuffer);


        [DllImport("gdi32.dll")]
        public static extern bool GdiFlush();
        [DllImport("gdi32.dll")]
        public static extern bool MoveToEx(
            IntPtr hdc,
            int X,
            int Y,
            [In, Out] POINT lpPoint);
        [DllImport("gdi32.dll")]
        public static extern bool LineTo(
            IntPtr hdc,
            int nXEnd,
            int nYEnd);



        [DllImport("user32.dll")]
        public static extern IntPtr BeginPaint(
            IntPtr hwnd,
            [Out] out PAINTSTRUCT lpPaint);
        [DllImport("user32.dll")]
        public static extern bool EndPaint(
            IntPtr hWnd,
            [In] ref PAINTSTRUCT lpPaint);

        [DllImport("user32.dll")]
        public static extern int FillRect(
            IntPtr hDC,
            [In] ref RECT lprc,
            IntPtr hbr);


        [DllImport("gdi32.dll")]
        public static extern IntPtr CreateSolidBrush(
            int crColor);



        #region Font enums
        public enum FontClipPrecision : uint
        {
            /// <summary>The default clipping behavior. </summary>
            DEFAULT = 0,
            /// <summary>This flag must be set for an embedded read-only font. </summary>
            EMBEDDED = 128,
            /// <summary>The direction of any rotations is determined by the coordinate system (or else all rotations are counterclockwise). </summary>
            LH_ANGLES = 16,
            /// <summary>Raster, vector, or TrueType font (used for enumeration only). </summary>
            STROKE = 2
        }
        public enum FontQuality : uint
        {
            /// <summary>Windows 95, 98, NT 4.0 or later, 2000: The font is always antialiased if possible. </summary>
            ANTIALIASED = 4,
            /// <summary>The default quality: the appearance of the font does not matter. </summary>
            DEFAULT = 0,
            /// <summary>The appearance of the font is less important then in PROOF_QUALITY. </summary>
            DRAFT = 1,
            /// <summary>Windows 95, 98, NT 4.0 or later, 2000: The font is never antialiased. </summary>
            NONANTIALIASED = 3,
            /// <summary>The quality of the appearance of the font is more important than exactly matching the specified font attributes.</summary>
            PROOF = 2
        }
        public enum FontPitchAndFamily : uint
        {
            /// <summary>The default pitch. </summary>
            DEFAULT = 0,
            /// <summary>Fixed pitch. </summary>
            FIXED = 1,
            /// <summary>Variable pitch.</summary>
            VARIABLE = 2,
            /// <summary>Showy, decorative font face. </summary>
            FF_DECORATIVE = 80,
            /// <summary>Do not care about the font face.</summary>
            FF_DONTCARE = 0,
            /// <summary>Roman font face (proportional-width, serif font). </summary>
            FF_ROMAN = 16,
            /// <summary>Script font face which imitates script handwriting. </summary>
            FF_SCRIPT = 64,
            /// <summary>Swiss font face (proportional-width, sans serif font). </summary>
            FF_SWISS = 32,
        }
        public enum FontWeight : uint
        {
            /// <summary>Default weight. </summary>
            DONTCARE = 0,
            /// <summary>Thin weight.</summary>
            THIN = 100,
            /// <summary>Extra-light weight. </summary>
            EXTRALIGHT = 200,
            /// <summary>Same as FW_EXTRALIGHT. </summary>
            ULTRALIGHT = 200,
            /// <summary></summary>
            LIGHT = 300,
            /// <summary>Normal weight. </summary>
            NORMAL = 400,
            /// <summary>Same as FW_NORMAL.</summary>
            REGULAR = 400,
            /// <summary>Medium weight. </summary>
            MEDIUM = 500,
            /// <summary>Semi-bold weight. </summary>
            SEMIBOLD = 600,
            /// <summary>Same As FW_SEMIBOLD. </summary>
            DEMIBOLD = 600,
            /// <summary>Bold weight. </summary>
            BOLD = 700,
            /// <summary>Extra-bold weight. </summary>
            EXTRABOLD = 800,
            /// <summary>Same as FW_EXTRABOLD. </summary>
            ULTRABOLD = 800,
            /// <summary>Heavy weight. </summary>
            HEAVY = 900,
            /// <summary>Same as FW_HEAVY. </summary>
            BLACK = 900
        }
        public enum FontCharset : uint
        {
            /// <summary>ANSI character set. </summary>
            ANSI = 0,
            /// <summary>Windows NT, 2000: Arabic character set. </summary>
            ARABIC = 178,
            /// <summary>Windows 95, 98: Baltic character set. </summary>
            BALTIC = 186,
            /// <summary>Chinese Big 5 character set. </summary>
            CHINESEBIG5 = 136,
            /// <summary>Default character set. </summary>
            DEFAULT = 1,
            /// <summary>Windows 95, 98: Eastern European character set. </summary>
            EASTEUROPE = 238,
            /// <summary>GB2312 character set. </summary>
            GB2312 = 134,
            /// <summary>Windows 95, 98: Greek character set. </summary>
            GREEK = 161,
            /// <summary>HANDEUL character set.</summary>
            HANGEULT = 129,
            /// <summary>Windows NT, 2000: Hebrew character set.</summary>
            HEBREW = 177,
            /// <summary>Windows 95, 98: Johab character set. </summary>
            JOHAB = 130,
            /// <summary>Windows 95, 98: Mac character set.</summary>
            MAC = 77,
            /// <summary>Original equipment manufacturer (OEM) character set.</summary>
            OEM = 255,
            /// <summary>Windows 95, 98: Russian character set. </summary>
            RUSSIAN = 204,
            /// <summary>ShiftJis character set. </summary>
            SHIFTJIS = 128,
            /// <summary>Symbol character set.</summary>
            SYMBOL = 2,
            /// <summary>Windows NT, 2000: Thai character set. </summary>
            THAI = 222,
            /// <summary>Windows 95, 98: Turkish character set. </summary>
            TURKISH = 162
        }
        public enum OutputPrecision : uint
        {
            /// <summary>The default font mapping behavior. </summary>
            DEFAULT = 0,
            /// <summary>Choose a device font if there are multiple fonts in the system with the same name. </summary>
            DEVICE = 5,
            /// <summary>Windows NT, 2000: Choose a TrueType or other outline-based font. </summary>
            OUTLINE = 8,
            /// <summary>Choose a raster font if there are multiple fonts in the system with the same name. </summary>
            RASTER = 6,
            /// <summary>Raster font (used for enumeration only). </summary>
            STRING = 1,
            /// <summary>Windows 95, 98: Vector font (used for enumeration only). Windows NT, 2000: TrueType, outline-based, or vector font (used for enumeration only). </summary>
            STROKE = 3,
            /// <summary>Choose only a TrueType font.</summary>
            TT_ONLY = 7,
            /// <summary>Choose a TrueType font if there are multiple fonts in the system with the same name. </summary>
            TT = 4
        }
        public enum FontItalic : uint
        {
            Yes = 1,
            No = 0,
        }
        public enum FontUnderline : uint
        {
            Yes = 1,
            No = 0,
        }
        public enum FontStrikeOut : uint
        {
            Yes = 1,
            No = 0,
        }
        #endregion



        #region DeviceCaps
        public enum DeviceCap
        {
            /// <summary>
            /// Device driver version
            /// </summary>
            DRIVERVERSION = 0,
            /// <summary>
            /// Device classification
            /// </summary>
            TECHNOLOGY = 2,
            /// <summary>
            /// Horizontal size in millimeters
            /// </summary>
            HORZSIZE = 4,
            /// <summary>
            /// Vertical size in millimeters
            /// </summary>
            VERTSIZE = 6,
            /// <summary>
            /// Horizontal width in pixels
            /// </summary>
            HORZRES = 8,
            /// <summary>
            /// Vertical height in pixels
            /// </summary>
            VERTRES = 10,
            /// <summary>
            /// Number of bits per pixel
            /// </summary>
            BITSPIXEL = 12,
            /// <summary>
            /// Number of planes
            /// </summary>
            PLANES = 14,
            /// <summary>
            /// Number of brushes the device has
            /// </summary>
            NUMBRUSHES = 16,
            /// <summary>
            /// Number of pens the device has
            /// </summary>
            NUMPENS = 18,
            /// <summary>
            /// Number of markers the device has
            /// </summary>
            NUMMARKERS = 20,
            /// <summary>
            /// Number of fonts the device has
            /// </summary>
            NUMFONTS = 22,
            /// <summary>
            /// Number of colors the device supports
            /// </summary>
            NUMCOLORS = 24,
            /// <summary>
            /// Size required for device descriptor
            /// </summary>
            PDEVICESIZE = 26,
            /// <summary>
            /// Curve capabilities
            /// </summary>
            CURVECAPS = 28,
            /// <summary>
            /// Line capabilities
            /// </summary>
            LINECAPS = 30,
            /// <summary>
            /// Polygonal capabilities
            /// </summary>
            POLYGONALCAPS = 32,
            /// <summary>
            /// Text capabilities
            /// </summary>
            TEXTCAPS = 34,
            /// <summary>
            /// Clipping capabilities
            /// </summary>
            CLIPCAPS = 36,
            /// <summary>
            /// Bitblt capabilities
            /// </summary>
            RASTERCAPS = 38,
            /// <summary>
            /// Length of the X leg
            /// </summary>
            ASPECTX = 40,
            /// <summary>
            /// Length of the Y leg
            /// </summary>
            ASPECTY = 42,
            /// <summary>
            /// Length of the hypotenuse
            /// </summary>
            ASPECTXY = 44,
            /// <summary>
            /// Shading and Blending caps
            /// </summary>
            SHADEBLENDCAPS = 45,

            /// <summary>
            /// Logical pixels inch in X
            /// </summary>
            LOGPIXELSX = 88,
            /// <summary>
            /// Logical pixels inch in Y
            /// </summary>
            LOGPIXELSY = 90,

            /// <summary>
            /// Number of entries in physical palette
            /// </summary>
            SIZEPALETTE = 104,
            /// <summary>
            /// Number of reserved entries in palette
            /// </summary>
            NUMRESERVED = 106,
            /// <summary>
            /// Actual color resolution
            /// </summary>
            COLORRES = 108,

            // Printing related DeviceCaps. These replace the appropriate Escapes
            /// <summary>
            /// Physical Width in device units
            /// </summary>
            PHYSICALWIDTH = 110,
            /// <summary>
            /// Physical Height in device units
            /// </summary>
            PHYSICALHEIGHT = 111,
            /// <summary>
            /// Physical Printable Area x margin
            /// </summary>
            PHYSICALOFFSETX = 112,
            /// <summary>
            /// Physical Printable Area y margin
            /// </summary>
            PHYSICALOFFSETY = 113,
            /// <summary>
            /// Scaling factor x
            /// </summary>
            SCALINGFACTORX = 114,
            /// <summary>
            /// Scaling factor y
            /// </summary>
            SCALINGFACTORY = 115,

            /// <summary>
            /// Current vertical refresh rate of the display device (for displays only) in Hz
            /// </summary>
            VREFRESH = 116,
            /// <summary>
            /// Horizontal width of entire desktop in pixels
            /// </summary>
            DESKTOPVERTRES = 117,
            /// <summary>
            /// Vertical height of entire desktop in pixels
            /// </summary>
            DESKTOPHORZRES = 118,
            /// <summary>
            /// Preferred blt alignment
            /// </summary>
            BLTALIGNMENT = 119
        }


        #endregion



        /*
#region BrushStyle i HatchStyle

    public enum HatchStyle
    {
        BackwardDiagonal = 3,
        Cross = 4,
        DarkDownwardDiagonal = 20,
        DarkHorizontal = 0x1d,
        DarkUpwardDiagonal = 0x15,
        DarkVertical = 0x1c,
        DashedDownwardDiagonal = 30,
        DashedHorizontal = 0x20,
        DashedUpwardDiagonal = 0x1f,
        DashedVertical = 0x21,
        DiagonalBrick = 0x26,
        DiagonalCross = 5,
        Divot = 0x2a,
        DottedDiamond = 0x2c,
        DottedGrid = 0x2b,
        ForwardDiagonal = 2,
        Horizontal = 0,
        HorizontalBrick = 0x27,
        LargeCheckerBoard = 50,
        LargeConfetti = 0x23,
        LargeGrid = 4,
        LightDownwardDiagonal = 0x12,
        LightHorizontal = 0x19,
        LightUpwardDiagonal = 0x13,
        LightVertical = 0x18,
        Max = 4,
        Min = 0,
        NarrowHorizontal = 0x1b,
        NarrowVertical = 0x1a,
        OutlinedDiamond = 0x33,
        Percent05 = 6,
        Percent10 = 7,
        Percent20 = 8,
        Percent25 = 9,
        Percent30 = 10,
        Percent40 = 11,
        Percent50 = 12,
        Percent60 = 13,
        Percent70 = 14,
        Percent75 = 15,
        Percent80 = 0x10,
        Percent90 = 0x11,
        Plaid = 0x29,
        Shingle = 0x2d,
        SmallCheckerBoard = 0x31,
        SmallConfetti = 0x22,
        SmallGrid = 0x30,
        SolidDiamond = 0x34,
        Sphere = 0x2f,
        Trellis = 0x2e,
        Vertical = 1,
        Wave = 0x25,
        Weave = 40,
        WideDownwardDiagonal = 0x16,
        WideUpwardDiagonal = 0x17,
        ZigZag = 0x24
    }



#endregion
*/

        [StructLayout(LayoutKind.Sequential)]
        public class POINT
        {
            public int Width;
            public int Height;
            public POINT() { }
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
        public class TEXTMETRIC
        {
            public int tmHeight;
            public int tmAscent;
            public int tmDescent;
            public int tmInternalLeading;
            public int tmExternalLeading;
            public int tmAveCharWidth;
            public int tmMaxCharWidth;
            public int tmWeight;
            public int tmOverhang;
            public int tmDigitizedAspectX;
            public int tmDigitizedAspectY;
            public char tmFirstChar;
            public char tmLastChar;
            public char tmDefaultChar;
            public char tmBreakChar;
            public byte tmItalic;
            public byte tmUnderlined;
            public byte tmStruckOut;
            public byte tmPitchAndFamily;
            public byte tmCharSet;
        }


        [StructLayout(LayoutKind.Sequential)]
        public struct SIZE
        {
            public int Width;
            public int Height;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct PAINTSTRUCT
        {
            public IntPtr hdc;
            public bool fErase;
            public RECT rcPaint;
            public bool fRestore;
            public bool fIncUpdate;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
            public byte[] rgbReserved;
        }


        [Serializable, StructLayout(LayoutKind.Sequential)]
        public struct RECT
        {
            public int Left;
            public int Top;
            public int Right;
            public int Bottom;

            public RECT(int left_, int top_, int right_, int bottom_)
            {
                Left = left_;
                Top = top_;
                Right = right_;
                Bottom = bottom_;
            }

            public int Height { get { return Bottom - Top; } }
            public int Width { get { return Right - Left; } }
            public Size Size { get { return new Size(Width, Height); } }

            public Point Location { get { return new Point(Left, Top); } }

            // Handy method for converting to a System.Drawing.Rectangle
            public Rectangle ToRectangle()
            { return Rectangle.FromLTRB(Left, Top, Right, Bottom); }

            public static RECT FromRectangle(Rectangle rectangle)
            {
                return new RECT(rectangle.Left, rectangle.Top, rectangle.Right, rectangle.Bottom);
            }

            public override int GetHashCode()
            {
                return Left ^ ((Top << 13) | (Top >> 0x13))
                  ^ ((Width << 0x1a) | (Width >> 6))
                  ^ ((Height << 7) | (Height >> 0x19));
            }

            #region Operator overloads

            public static implicit operator Rectangle(RECT rect)
            {
                return rect.ToRectangle();
            }

            public static implicit operator RECT(Rectangle rect)
            {
                return FromRectangle(rect);
            }

            #endregion
        }
    }
}
