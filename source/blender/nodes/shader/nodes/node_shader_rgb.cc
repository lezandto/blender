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

/* **************** RGB ******************** */
static bNodeSocketTemplate sh_node_rgb_out[] = {
    {SOCK_RGBA, N_("Color"), 0.5f, 0.5f, 0.5f, 1.0f},
    {-1, ""},
};

static int gpu_shader_rgb(GPUMaterial *mat,
                          bNode *node,
                          bNodeExecData *UNUSED(execdata),
                          GPUNodeStack *in,
                          GPUNodeStack *out)
{
  GPUNodeLink *link = GPU_uniformbuf_link_out(mat, node, out, 0);
  return GPU_stack_link(mat, node, "set_rgba", in, out, link);
}

void register_node_type_sh_rgb(void)
{
  static bNodeType ntype;

  sh_node_type_base(&ntype, SH_NODE_RGB, "RGB", NODE_CLASS_INPUT, 0);
  node_type_socket_templates(&ntype, nullptr, sh_node_rgb_out);
  node_type_gpu(&ntype, gpu_shader_rgb);

  nodeRegisterType(&ntype);
}
