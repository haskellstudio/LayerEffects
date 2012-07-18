//------------------------------------------------------------------------------
/*
  LayerEffects for JUCE

  Official project location:
  https://github.com/vinniefalco/LayerEffects

  ------------------------------------------------------------------------------

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
  Copyright (c) 2012 by Vinnie Falco

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/
//------------------------------------------------------------------------------

CTextDemo::CTextDemo ()
{
  setSize (512, 512);
}

CTextDemo::~CTextDemo ()
{
}

void CTextDemo::paint (Graphics& g)
{
  Rectangle <int> const b (getLocalBounds ());

#if 0
  g.setGradientFill (ColourGradient (
    Colours::red, b.getX (), b.getY (),
    Colours::yellow, b.getRight (), b.getBottom (),
    false));
  g.setFont (b.getHeight () / 3.f);
  g.drawFittedText ("Layer\nEffects",
    b, Justification::centred, 2);

#else
  // Photoshop "Background" Layer
  vf::BackgroundContext bc (g, b);
  bc.setGradientFill (ColourGradient (
    Colours::black, float (b.getX ()), float (b.getY ()),
    Colours::white, float (b.getX ()), float (b.getBottom ()), false));
  bc.fillRect (b);

  // Photoshop Layer + Mask, with Layer Effects
  vf::LayerContext lc (bc, b);
  vf::LayerContext::Options& opt (lc.getOptions ());

  lc.setGradientFill (ColourGradient (
    Colours::red, float (b.getX ()), float (b.getY ()),
    Colours::yellow, float (b.getRight ()), float (b.getBottom ()),
    false));
  lc.setFont (Font ("Impact", b.getHeight () / 3.f, Font::plain));
  lc.drawFittedText ("Layer\nEffects", b, Justification::centred, 2);

  opt.general.mode = vf::normal;
  opt.general.opacity = 1;
  opt.fill.opacity = 1;

  opt.dropShadow.active   = true;
  opt.dropShadow.mode     = vf::overlay;
  opt.dropShadow.colour   = Colours::black;
  opt.dropShadow.angle    = 2*3.14159 * 135 / 360;
  opt.dropShadow.distance = 4;
  opt.dropShadow.spread   = 0.5;
  opt.dropShadow.size     = 8;
  opt.dropShadow.knockout = true;

  opt.innerShadow.active   = true;
  opt.innerShadow.mode     = vf::lighten;
  opt.innerShadow.colour   = Colours::yellow;
  opt.innerShadow.angle    = 2*3.14159 * 135 / 360;
  opt.innerShadow.distance = 6;
  opt.innerShadow.choke    = 0.5;
  opt.innerShadow.size     = 4;

#endif
}

void CTextDemo::mouseDown (MouseEvent const& e)
{
  repaint ();
}
