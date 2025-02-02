/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2005 Blender Foundation.
 * All rights reserved.
 */

/** \file
 * \ingroup shdnodes
 */

#include "node_shader_util.hh"

/* **************** VALUE SQUEEZE ******************** */
static bNodeSocketTemplate sh_node_squeeze_in[] = {
    {SOCK_FLOAT, N_("Value"), 0.0f, 0.0f, 0.0f, 0.0f, -100.0f, 100.0f, PROP_NONE},
    {SOCK_FLOAT, N_("Width"), 1.0f, 0.0f, 0.0f, 0.0f, -100.0f, 100.0f, PROP_NONE},
    {SOCK_FLOAT, N_("Center"), 0.0f, 0.0f, 0.0f, 0.0f, -100.0f, 100.0f, PROP_NONE},
    {-1, ""}};

static bNodeSocketTemplate sh_node_squeeze_out[] = {{SOCK_FLOAT, N_("Value")}, {-1, ""}};

static void node_shader_exec_squeeze(void *UNUSED(data),
                                     int UNUSED(thread),
                                     bNode *UNUSED(node),
                                     bNodeExecData *UNUSED(execdata),
                                     bNodeStack **in,
                                     bNodeStack **out)
{
  float vec[3];

  nodestack_get_vec(vec, SOCK_FLOAT, in[0]);
  nodestack_get_vec(vec + 1, SOCK_FLOAT, in[1]);
  nodestack_get_vec(vec + 2, SOCK_FLOAT, in[2]);

  out[0]->vec[0] = 1.0f / (1.0f + powf(M_E, -((vec[0] - vec[2]) * vec[1])));
}

static int gpu_shader_squeeze(GPUMaterial *mat,
                              bNode *node,
                              bNodeExecData *UNUSED(execdata),
                              GPUNodeStack *in,
                              GPUNodeStack *out)
{
  return GPU_stack_link(mat, node, "squeeze", in, out);
}

void register_node_type_sh_squeeze(void)
{
  static bNodeType ntype;

  sh_node_type_base(&ntype, SH_NODE_SQUEEZE, "Squeeze Value", NODE_CLASS_CONVERTER, 0);
  node_type_socket_templates(&ntype, sh_node_squeeze_in, sh_node_squeeze_out);
  node_type_storage(&ntype, "", nullptr, nullptr);
  node_type_exec(&ntype, nullptr, nullptr, node_shader_exec_squeeze);
  node_type_gpu(&ntype, gpu_shader_squeeze);

  nodeRegisterType(&ntype);
}
