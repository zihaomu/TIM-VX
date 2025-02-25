/****************************************************************************
 *
 *    Copyright (c) 2020 Vivante Corporation
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
#ifndef TIM_LAYOUT_INFER_PAD_LAYOUT_INFERENCE_H_
#define TIM_LAYOUT_INFER_PAD_LAYOUT_INFERENCE_H_

#include "tim/vx/ops/pad.h"

#include "ops/op_layout_inference.h"
#include "permute_vector.h"
#include "operation_private.h"
namespace tim {
namespace transform {
class PadLayoutInfer : public OpLayoutInfer {
 public:
  PadLayoutInfer(
      const std::shared_ptr<vx::Operation> op,
      std::shared_ptr<layout_inference_impl::LayoutInferContext>& context)
      : OpLayoutInfer(op, context) {}

  void OnInputs(
      std::vector<std::shared_ptr<vx::Tensor>>& next_tensors) override {
    assert(op_->impl()->InputsTensor().size() == 1);
    auto i_src = op_->impl()->InputsTensor()[0];
    auto input_pv = context_->GetPermuteVector(i_src);

    uint32_t dim_num = op_->impl()->node()->nn_param.pad.dim_num;
    std::vector<uint32_t> front_size(dim_num);
    std::vector<uint32_t> back_size(dim_num);
    memcpy(front_size.data(), op_->impl()->node()->nn_param.pad.front_size,
           sizeof(uint32_t) * dim_num);
    memcpy(back_size.data(), op_->impl()->node()->nn_param.pad.back_size,
           sizeof(uint32_t) * dim_num);
    int32_t pad_value = op_->impl()->node()->nn_param.pad.const_val;

    if (!input_pv->IsAligned()) {
      front_size = MapMultipleAxis(input_pv->AsStdVec(), front_size);
      back_size = MapMultipleAxis(input_pv->AsStdVec(), back_size);
    }

    auto pad = context_->infer_graph_->CreateOperation<vx::ops::Pad>(
        front_size, back_size, pad_value);
    auto out_infer = CreateOutputsTensor(input_pv);
    (*pad).BindInput(context_->GetMapedTensor(i_src));
    (*pad).BindOutput(out_infer[0]);
    context_->SetPermuteVector(op_->impl()->OutputsTensor()[0], input_pv);
    next_tensors.push_back(op_->impl()->OutputsTensor()[0]);
  }
};

}  // namespace transform
}  // namespace tim

#endif