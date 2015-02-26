/*=========================================================================
 *
 *  Copyright SINAPSE: Scalable Informatics for Neuroscience, Processing and Software Engineering
 *            The University of Iowa
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "BRAINSThreadControl.h"

namespace BRAINSUtils
{
StackPushITKDefaultNumberOfThreads::StackPushITKDefaultNumberOfThreads(const int desiredCount) :
  m_originalThreadValue( itk::MultiThreader::GetGlobalDefaultNumberOfThreads() )
{
  itk::MultiThreader::SetGlobalDefaultUseThreadPool(true);
  int threadCount(-1);

  if( desiredCount > 0 )  // NOTE: Default is -1, which then uses the ITK default.
    {
    // Respect the users request irregardless of what environmental variables are set to.
    threadCount = desiredCount;
    }
  else
    {                                          // If user set desiredCount <= 0, then use evnironmentanl or internal
                                               // ITKv4 values.
    threadCount = this->m_originalThreadValue; // This is the old default.
      {                                        // Process the NSLOTS environmental varialble set by the SGE batch
                                               // processing system
      int         NSLOTSThreadCount(-1);
      std::string numThreads;
      if( itksys::SystemTools::GetEnv("NSLOTS", numThreads) )
        {
        std::istringstream s(numThreads, std::istringstream::in);
        s >> NSLOTSThreadCount;
        }
      if( NSLOTSThreadCount > threadCount )
        {
        threadCount = NSLOTSThreadCount;
        }
      }
    }
  if( threadCount > 0 )
    {
    itk::MultiThreader::SetGlobalDefaultNumberOfThreads(threadCount);
    }
}

StackPushITKDefaultNumberOfThreads::~StackPushITKDefaultNumberOfThreads()
{
  itk::MultiThreader::SetGlobalDefaultNumberOfThreads(this->m_originalThreadValue);
}
}
