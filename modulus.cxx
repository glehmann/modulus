#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkModulusImageFilter.h"
#include "itkDanielssonDistanceMapImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkCommand.h"
#include "itkNumericTraits.h"

template < class TFilter >
class ProgressCallback : public itk::Command
{
public:
  typedef ProgressCallback   Self;
  typedef itk::Command  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  itkTypeMacro( IterationCallback, Superclass );
  itkNewMacro( Self );
  
  /** Type defining the optimizer. */
  typedef    TFilter     FilterType;

  /** Method to specify the optimizer. */
  void SetFilter( FilterType * filter )
    {
    m_Filter = filter;
    m_Filter->AddObserver( itk::ProgressEvent(), this );
    }

  /** Execute method will print data at each iteration */
  void Execute(itk::Object *caller, const itk::EventObject & event)
    {
    Execute( (const itk::Object *)caller, event);
    }
    
  void Execute(const itk::Object *, const itk::EventObject & event)
    {
    std::cout << m_Filter->GetNameOfClass() << ": " << m_Filter->GetProgress() << std::endl;
    }

protected:
  ProgressCallback() {};
  itk::WeakPointer<FilterType>   m_Filter;
};


int main(int, char * argv[])
{
  const int dim = 2;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  // get the distance map inside the spots
  // spot are already black so there is no need to invert the image
  typedef itk::DanielssonDistanceMapImageFilter< IType, IType > DistanceFilter;
  DistanceFilter::Pointer distance = DistanceFilter::New();
  distance->SetInput( reader->GetOutput() );

  typedef itk::ModulusImageFilter< IType, IType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( distance->GetOutput() );
  filter->SetDividend( 8 );

  typedef ProgressCallback< FilterType > ProgressType;
  ProgressType::Pointer progress = ProgressType::New();
  progress->SetFilter( filter );

  typedef itk::RescaleIntensityImageFilter< IType, IType > ThresholdType;
  ThresholdType::Pointer rescale = ThresholdType::New();
  rescale->SetInput( filter->GetOutput() );
  rescale->SetOutputMaximum( itk::NumericTraits< PType >::max() );
  rescale->SetOutputMinimum( itk::NumericTraits< PType >::NonpositiveMin() );

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( rescale->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  rescale->SetInput( distance->GetOutput() );
  writer->SetFileName( argv[3] );
  writer->Update();

  return 0;
}

