/****************************************************************************
*
*    Copyright (c) 2021 Vivante Corporation
*
*    Permission is hereby granted, free of charge, to any person obtaining a
*    copy of this software and associated documentation files (the "Software"),
*    to deal in the Software without restriction, including without limitation
*    the rights to use, copy, modify, merge, publish, distribute, sublicense,
*    and/or sell copies of the Software, and to permit persons to whom the
*    Software is furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in
*    all copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
*    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
*    DEALINGS IN THE SOFTWARE.
*
*****************************************************************************/
#ifndef TIM_VX_OPS_SVDF_H_
#define TIM_VX_OPS_SVDF_H_

#include <array>

#include "tim/vx/operation.h"
#include "tim/vx/types.h"

namespace tim {
namespace vx {
namespace ops {

/**
 * ## Svdf
 *
 * Performs an 2-D pooling operation.
 *
 * - rank : The rank of the SVD approximation.
 * - num_units : corresponds to the number of units.
 * - spectrogram_length : corresponds to the fixed-size of the memory.
 */

class Svdf : public Operation {
 public:
  Svdf(Graph* graph, int32_t rank, int32_t num_units, int32_t spectrogram_length);

  std::shared_ptr<Operation> Clone(std::shared_ptr<Graph>& graph) const override;
};

}  // namespace ops
}  // namespace vx
}  // namespace tim

#endif /* TIM_VX_OPS_SVDF_H_ */