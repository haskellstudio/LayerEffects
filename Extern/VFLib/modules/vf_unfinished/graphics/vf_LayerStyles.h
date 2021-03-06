/*============================================================================*/
/*
  VFLib: https://github.com/vinniefalco/VFLib

  Copyright (C) 2008 by Vinnie Falco <vinnie.falco@gmail.com>

  This library contains portions of other open source products covered by
  separate licenses. Please see the corresponding source files for specific
  terms.
  
  VFLib is provided under the terms of The MIT License (MIT):

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
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.
*/
/*============================================================================*/

/** Common routines for Layer Styles.
*/
class LayerStyles
{
public:
  /** Calculate blur and dilate settings.
  */
  struct BoxBlurAndDilateSettings
  {
    BoxBlurAndDilateSettings (int sizeInPixels, float spread)
    {
      m_enlargePixels = sizeInPixels + 2;
      m_dilatePixels = int (sizeInPixels * spread + 0.5);

      int blurPixels = sizeInPixels - m_dilatePixels;

      // Photoshop fudge factor by Brian Fiete
      float const fudge = 1.85f - 0.45f * std::min (1.0f, blurPixels / 10.f);
      m_boxBlurRadius = std::max (blurPixels - fudge, 0.f);
    }

    int getEnlargePixels () const
    {
      return m_enlargePixels;
    }

    int getDilatePixels () const
    {
      return m_dilatePixels;
    }

    float getBoxBlurRadius () const
    {
      return m_boxBlurRadius;
    }

  private:
    int m_enlargePixels;
    int m_dilatePixels;
    float m_boxBlurRadius;
  };

  /** Produce grayscale dilation.
  */
  struct GrayscaleDilation
  {
    template <class In, class Out>
    void operator () (
      In in,
      int inRows,
      int inCols,
      Out out,
      int outRows,
      int outCols,
      int outX,
      int outY,
      int size) const
    {
      //if (size > 0)
      {
        ChamferDistance () (
          in,
          inRows,
          inCols,
          Output (out, size),
          outRows,
          outCols,
          outX,
          outY,
          ChamferDistance::MaskInit ());
      }
#if 0
      else
      {
        assert (0);
        for (int y = 0; y < outRows; ++y)
        {
          for (int x = 0; x < outCols; ++x)
          {
            out (x, y) = in (x, y) * 256;
          }
        }
      }
#endif
    }

  private:
    struct Output
    {
      Output (Map2D <int> dest, int size)
        : m_dest (dest)
        , m_size (size * 256)
        , m_sizePlusOne (m_size + 256)
      {
      }

      // Distance has 8 bits fixed precision
      //
      template <class T>
      void operator () (int x, int y, T distance)
      {
        if (distance <= m_size)
          m_dest (x, y) = 255 * 256;
        else if (distance < m_sizePlusOne)
          m_dest (x, y) = (255 - (distance - m_size)) * 256;
        else
          m_dest (x, y) = 0;
      }

    private:
      Map2D <int> m_dest;
      int m_size;
      int m_sizePlusOne;
    };
  };

  /** Produce grayscale dilation.
  */
  struct GrayscaleDilation2
  {
    template <class Out>
    void operator () (
      int const inRows,
      int const inCols,
      unsigned char* inBaseAddr,
      int const inRowBytes,
      int const inColBytes,
      int inX,
      int inY,
      Out out,
      int outRows,
      int outCols,
      int size) const
    {
      //if (size > 0)
      {
        ChamferDistance2 () (
          inRows,
          inCols,
          inBaseAddr,
          inRowBytes,
          inColBytes,
          inX,
          inY,
          Output (out, size),
          outRows,
          outCols,
          ChamferDistance::MaskInit ());
      }
#if 0
      else
      {
        assert (0);
        for (int y = 0; y < outRows; ++y)
        {
          for (int x = 0; x < outCols; ++x)
          {
            out (x, y) = in (x, y) * 256;
          }
        }
      }
#endif
    }

  private:
    struct Output
    {
      Output (Map2D <int> dest, int size)
        : m_dest (dest)
        , m_size (size * 256)
        , m_sizePlusOne (m_size + 256)
      {
      }

      // Distance has 8 bits fixed precision
      //
      template <class T>
      void operator () (int x, int y, T distance)
      {
        if (distance <= m_size)
          m_dest (x, y) = 255 * 256;
        else if (distance < m_sizePlusOne)
          m_dest (x, y) = (255 - (distance - m_size)) * 256;
        else
          m_dest (x, y) = 0;
      }

    private:
      Map2D <int> m_dest;
      int m_size;
      int m_sizePlusOne;
    };
  };

  /** Produce grayscale erosion.
  */
  struct GrayscaleErosion
  {
    template <class In, class Out>
    void operator () (In in, Out out, int width, int height, int size) const
    {
      if (size > 0)
      {
        ChamferDistance () (
          in,
          height,
          width,
          Output (out, size),
          height,
          width,
          0,
          0,
          ChamferDistance::InverseMaskInit ());
      }
      else
      {
        for (int y = 0; y < height; ++y)
        {
          for (int x = 0; x < width; ++x)
          {
            out (x, y) = (255- in (x, y)) * 256;
          }
        }
      }
    }

  private:
    template <class In>
    struct Input
    {
      Input (In src) : m_src (src)
      {
      }

      int operator() (int x, int y) const
      {
        return 255 - m_src (x, y);
      }

    private:
      In m_src;
    };

    struct Output
    {
      Output (Map2D <int> dest, int size)
        : m_dest (dest)
        , m_size (size * 256)
        , m_sizePlusOne (m_size + 256)
      {
      }

      // Distance has 8 bits fixed precision
      //
      template <class T>
      void operator () (int x, int y, T distance)
      {
        if (distance <= m_size)
          m_dest (x, y) = 255 * 256;
        else if (distance < m_sizePlusOne)
          m_dest (x, y) = (255 - (distance - m_size)) * 256;
        else
          m_dest (x, y) = 0;
      }

    private:
      Map2D <int> m_dest;
      int m_size;
      int m_sizePlusOne;
    };
  };

  /** Apply a distance transform.
  */
  struct DistanceMap
  {
    template <class In, class Out>
    void operator () (In in, Out out, int width, int height, int size) const
    {
      if (size > 0)
      {
        ChamferDistance () (
          in,
          height,
          width,
          Output <Out> (out, size),
          height,
          width,
          0,
          0,
          ChamferDistance::MaskInit ());
      }
      else
      {
        for (int y = 0; y < height; ++y)
        {
          for (int x = 0; x < width; ++x)
          {
            out (x, y) = in (x, y) * 256;
          }
        }
      }
    }

  private:
    template <class T>
    struct Output
    {
      Output (T dest, int size)
        : m_dest (dest)
        , m_size (size * 256)
        , m_sizePlusOne (m_size + 256)
      {
      }

      // Distance has 8 bits fixed precision
      //
      template <class U>
      void operator () (int x, int y, U distance)
      {
        if (distance <= m_size)
          m_dest (x, y) = distance * 256 / m_size;
        else
          m_dest (x, y) = 0;
      }

    private:
      T m_dest;
      int m_size;
      int m_sizePlusOne;
    };
  };
};
