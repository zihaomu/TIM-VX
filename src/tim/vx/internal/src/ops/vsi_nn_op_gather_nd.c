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
#include <string.h>
#include <stdlib.h>

#include "vsi_nn_types.h"
#include "vsi_nn_log.h"
#include "vsi_nn_node.h"
#include "vsi_nn_prv.h"
#include "vsi_nn_ops.h"
#include "vsi_nn_tensor.h"
#include "vsi_nn_error.h"
#include "utils/vsi_nn_util.h"
#include "kernel/vsi_nn_kernel.h"
#include "utils/vsi_nn_constraint_check.h"

#define _ARG_NUM            (2)
#define _INPUT_NUM          (2)
#define _OUTPUT_NUM         (1)

static vsi_status op_compute
    (
    vsi_nn_node_t * self,
    vsi_nn_tensor_t ** inputs,
    vsi_nn_tensor_t ** outputs
    )
{
    vsi_status status = VSI_FAILURE;
    vsi_nn_kernel_param_t * param = NULL;
    vsi_nn_kernel_node_t    n = NULL;
    vsi_size_t i = 0;
    vsi_size_t block_size = 1, coord_dim = 1;
    vsi_size_t *input_size = inputs[0]->attr.size;
    vsi_size_t dims_num = inputs[0]->attr.dim_num;

    if(inputs[1]->attr.dim_num > 1)
    {
        coord_dim = inputs[1]->attr.size[0];
    }
    if( coord_dim > 3 )
    {
        CHECK_STATUS(status);
        return status;
    }

    param =vsi_nn_kernel_param_create();

    for(i = 0; i < dims_num - coord_dim; ++i)
    {
        block_size *= input_size[i];
    }

    vsi_nn_kernel_param_add_int32( param, "block_size", (int32_t)block_size );
    vsi_nn_kernel_param_add_int32( param, "coord_dim", (int32_t)coord_dim );
    n = vsi_nn_kernel_selector( self->graph, "gather_nd", inputs, 2, outputs, 1, param );
    if( n != NULL )
    {
        self->n = (vx_node)n;
        status = VSI_SUCCESS;
    }

    if(param != NULL)
    {
        vsi_nn_kernel_param_release( &param );
    }

    return status;
} /* op_compute() */

static vsi_bool op_check
    (
    vsi_nn_node_t * self,
    vsi_nn_tensor_t ** inputs,
    vsi_nn_tensor_t ** outputs
    )
{
    BEGIN_IO_TYPE_DECL(GATHER_ND, 2, 1)
        IO_TYPE(D_I32,  D_I32,  D_I32)
        IO_TYPE(D_F32,  D_I32,  D_F32)
        IO_TYPE(D_F16,  D_I32,  D_U8|Q_ASYM)
        IO_TYPE(D_F16,  D_I32,  D_I16|Q_DFP)
        IO_TYPE(D_F16,  D_I32,  D_I8|Q_DFP)
        IO_TYPE(D_F16,  D_I32,  D_F16)
        IO_TYPE(D_BF16, D_I32,  D_BF16)
        IO_TYPE(D_U8|Q_ASYM, D_I32, D_U8|Q_ASYM)
        IO_TYPE(D_U8|Q_ASYM, D_I32, D_F16)
        IO_TYPE(D_I8|Q_DFP,  D_I32,  D_I8|Q_DFP)
        IO_TYPE(D_I8|Q_DFP,  D_I32,  D_F16)
        IO_TYPE(D_I16|Q_DFP, D_I32, D_I16|Q_DFP)
        IO_TYPE(D_I16|Q_DFP, D_I32, D_F16)
    END_IO_TYPE_DECL(GATHER_ND)
    if(!VALIDATE_OP_IO_TYPES(GATHER_ND, self, inputs, self->input.num, outputs, self->output.num)) {
        char* desc = generate_op_io_types_desc(inputs,
                self->input.num, outputs, self->output.num);
        VSILOGE("Inputs/Outputs data type not support: %s", desc);
        destroy_op_io_types_desc(desc);
        return FALSE;
    }

    return TRUE;
} /* op_check() */

static vsi_bool op_setup
    (
    vsi_nn_node_t * self,
    vsi_nn_tensor_t ** inputs,
    vsi_nn_tensor_t ** outputs
    )
{
    /* TODO: Add code to comput outputs' shape. */
    vsi_size_t i = 0;

    if( VSI_NN_DIM_AUTO == outputs[0]->attr.dim_num )
    {
        vsi_size_t j = 0, coord_dim = 1;
        if(inputs[1]->attr.dim_num > 1)
        {
            coord_dim = inputs[1]->attr.size[0];
        }

        for (i = 0; i < (uint32_t)inputs[0]->attr.dim_num - coord_dim; i++)
        {
            outputs[0]->attr.size[j++] = inputs[0]->attr.size[i];
        }
        for (i = 1; i < inputs[1]->attr.dim_num; i++)
        {
            outputs[0]->attr.size[j++] = inputs[1]->attr.size[i];
        }
        if(inputs[1]->attr.dim_num == 1)
        {
            outputs[0]->attr.size[j++] = inputs[1]->attr.size[0];
        }
        outputs[0]->attr.dim_num = (uint32_t)j;
    }
    return TRUE;
} /* op_setup() */

static vsi_status op_deinit
    (
    vsi_nn_node_t * self
    )
{
    vsi_nn_op_common_deinit(self);

    return VSI_SUCCESS;
} /* op_deinit() */

#ifdef __cplusplus
extern "C" {
#endif
/* Registrar */
DEF_OP_REG
    (
    /* op_name    */ GATHER_ND,
    /* init       */ NULL,
    /* compute    */ op_compute,
    /* deinit     */ op_deinit,
    /* check      */ op_check,
    /* setup      */ op_setup,
    /* optimize   */ NULL,
    /* input_num  */ _INPUT_NUM,
    /* output_num */ _OUTPUT_NUM
    );
#ifdef __cplusplus
}
#endif
