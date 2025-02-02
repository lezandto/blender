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
 * The Original Code is Copyright (C) 2018 Blender Foundation.
 * All rights reserved.
 */

#include "node_shader_util.hh"

/* **************** OUTPUT ******************** */

/* Color, melanin and absorption coefficient default to approximately same brownish hair. */
static bNodeSocketTemplate sh_node_bsdf_hair_principled_in[] = {
    {SOCK_RGBA, N_("Color"), 0.017513f, 0.005763f, 0.002059f, 1.0f, 0.0f, 1.0f},
    {SOCK_FLOAT, N_("Melanin"), 0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, PROP_FACTOR},
    {SOCK_FLOAT, N_("Melanin Redness"), 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, PROP_FACTOR},
    {SOCK_RGBA, N_("Tint"), 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
    {SOCK_VECTOR, N_("Absorption Coefficient"), 0.245531f, 0.52f, 1.365f, 0.0f, 0.0f, 1000.0f},
    {SOCK_FLOAT, N_("Roughness"), 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, PROP_FACTOR},
    {SOCK_FLOAT, N_("Radial Roughness"), 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, PROP_FACTOR},
    {SOCK_FLOAT, N_("Coat"), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, PROP_FACTOR},
    {SOCK_FLOAT, N_("IOR"), 1.55f, 0.0f, 0.0f, 0.0f, 0.0f, 1000.0f},
    {SOCK_FLOAT,
     N_("Offset"),
     2.0f * ((float)M_PI) / 180.0f,
     0.0f,
     0.0f,
     0.0f,
     -M_PI_2,
     M_PI_2,
     PROP_ANGLE},
    {SOCK_FLOAT, N_("Random Color"), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, PROP_FACTOR},
    {SOCK_FLOAT, N_("Random Roughness"), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, PROP_FACTOR},
    {SOCK_FLOAT, N_("Random"), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, PROP_NONE, SOCK_HIDE_VALUE},
    {-1, ""},
};

static bNodeSocketTemplate sh_node_bsdf_hair_principled_out[] = {
    {SOCK_SHADER, N_("BSDF")},
    {-1, ""},
};

/* Initialize the custom Parametrization property to Color. */
static void node_shader_init_hair_principled(bNodeTree *UNUSED(ntree), bNode *node)
{
  node->custom1 = SHD_PRINCIPLED_HAIR_REFLECTANCE;
}

/* Triggers (in)visibility of some sockets when changing Parametrization. */
static void node_shader_update_hair_principled(bNodeTree *ntree, bNode *node)
{
  int parametrization = node->custom1;

  LISTBASE_FOREACH (bNodeSocket *, sock, &node->inputs) {
    if (STREQ(sock->name, "Color")) {
      nodeSetSocketAvailability(ntree, sock, parametrization == SHD_PRINCIPLED_HAIR_REFLECTANCE);
    }
    else if (STREQ(sock->name, "Melanin")) {
      nodeSetSocketAvailability(
          ntree, sock, parametrization == SHD_PRINCIPLED_HAIR_PIGMENT_CONCENTRATION);
    }
    else if (STREQ(sock->name, "Melanin Redness")) {
      nodeSetSocketAvailability(
          ntree, sock, parametrization == SHD_PRINCIPLED_HAIR_PIGMENT_CONCENTRATION);
    }
    else if (STREQ(sock->name, "Tint")) {
      nodeSetSocketAvailability(
          ntree, sock, parametrization == SHD_PRINCIPLED_HAIR_PIGMENT_CONCENTRATION);
    }
    else if (STREQ(sock->name, "Absorption Coefficient")) {
      nodeSetSocketAvailability(
          ntree, sock, parametrization == SHD_PRINCIPLED_HAIR_DIRECT_ABSORPTION);
    }
    else if (STREQ(sock->name, "Random Color")) {
      nodeSetSocketAvailability(
          ntree, sock, parametrization == SHD_PRINCIPLED_HAIR_PIGMENT_CONCENTRATION);
    }
  }
}

/* node type definition */
void register_node_type_sh_bsdf_hair_principled(void)
{
  static bNodeType ntype;

  sh_node_type_base(
      &ntype, SH_NODE_BSDF_HAIR_PRINCIPLED, "Principled Hair BSDF", NODE_CLASS_SHADER, 0);
  node_type_socket_templates(
      &ntype, sh_node_bsdf_hair_principled_in, sh_node_bsdf_hair_principled_out);
  node_type_size_preset(&ntype, NODE_SIZE_LARGE);
  node_type_init(&ntype, node_shader_init_hair_principled);
  node_type_storage(&ntype, "", nullptr, nullptr);
  node_type_update(&ntype, node_shader_update_hair_principled);

  nodeRegisterType(&ntype);
}
