/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkModulusImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2004/11/29 22:18:43 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkModulusImageFilter_h
#define __itkModulusImageFilter_h

#include "itkUnaryFunctorImageFilter.h"

namespace itk
{

namespace Functor {  
 
template< typename TInput, typename  TOutput>
class ModulusTransform
{
public:
  ModulusTransform() {}
  ~ModulusTransform() {}
  void SetDividend( TOutput dividend ) { m_Dividend = dividend; }
  inline TOutput operator()( const TInput & x )
  {
    TOutput  result = static_cast<TOutput>( x % m_Dividend );
    return result;
  }
private:
  TInput  m_Dividend;
}; 

}  // end namespace functor


/** \class ModulusImageFilter
 * \brief Computes the modulus (x % dividend) pixel-wise
 *
 * The input pixel type must support the c++ modulus operator (%).
 *
 * \author Ga�tan Lehmann. Biologie du D�veloppement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \ingroup IntensityImageFilters  Multithreaded
 *
 */
template <typename  TInputImage, typename  TOutputImage=TInputImage>
class ITK_EXPORT ModulusImageFilter :
    public
UnaryFunctorImageFilter<TInputImage,TOutputImage, 
                        Functor::ModulusTransform< 
  typename TInputImage::PixelType, 
  typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef ModulusImageFilter  Self;
  typedef UnaryFunctorImageFilter<TInputImage,TOutputImage, 
                                  Functor::ModulusTransform< 
    typename TInputImage::PixelType, 
    typename TOutputImage::PixelType> >  Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  typedef typename TOutputImage::PixelType OutputPixelType;
  typedef typename TInputImage::PixelType  InputPixelType;
  typedef typename NumericTraits<InputPixelType>::RealType RealType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Set/Get the dividend */
  itkSetMacro( Dividend, InputPixelType );
  itkGetConstReferenceMacro( Dividend, InputPixelType );

  /** Print internal ivars */
  void PrintSelf(std::ostream& os, Indent indent) const;
  
  /** Process to execute before entering the multithreaded section */
  void BeforeThreadedGenerateData(void);

protected:
  ModulusImageFilter();
  virtual ~ModulusImageFilter() {};

private:
  ModulusImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  InputPixelType        m_Dividend;
};


  
} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkModulusImageFilter.txx"
#endif
  
#endif
