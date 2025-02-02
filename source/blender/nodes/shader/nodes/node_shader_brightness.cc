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
 * The Original Code is Copyright (C) 2006 Blender Foundation.
 * All rights reserved.
 */

#include "node_shader_util.hh"

/* **************** Bright and contrast  ******************** */

static bNodeSocketTemplate sh_node_brightcontrast_in[] = {
    {SOCK_RGBA, N_("Color"), 1.0f, 1.0f, 1.0f, 1.0f},
    {SOCK_FLOAT, N_("Bright"), 0.0f, 0.0f, 0.0f, 0.0f, -100.0f, 100.0f, PROP_NONE},
    {SOCK_FLOAT, N_("Contrast"), 0.0f, 0.0f, 0.0f, 0.0f, -100.0f, 100.0f, PROP_NONE},
    {-1, ""},
};

static bNodeSocketTemplate sh_node_brightcontrast_out[] = {
    {SOCK_RGBA, N_("Color")},
    {-1, ""},
};

static int gpu_shader_brightcontrast(GPUMaterial *mat,
                                     bNode *node,
                                     bNodeExecData *UNUSED(execdata),
                                     GPUNodeStack *in,
                                     GPUNodeStack *out)
{
  return GPU_stack_link(mat, node, "brightness_contrast", in, out);
}

void register_node_type_sh_brightcontrast(void)
{
  static bNodeType ntype;

  sh_node_type_base(&ntype, SH_NODE_BRIGHTCONTRAST, "Bright/Contrast", NODE_CLASS_OP_COLOR, 0);
  node_type_socket_templates(&ntype, sh_node_brightcontrast_in, sh_node_brightcontrast_out);
  node_type_init(&ntype, nullptr);
  node_type_storage(&ntype, "", nullptr, nullptr);
  node_type_gpu(&ntype, gpu_shader_brightcontrast);

  nodeRegisterType(&ntype);
}
