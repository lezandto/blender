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
 * The Original Code is Copyright (C) 2014 Blender Foundation.
 * All rights reserved.
 */

/** \file
 * \ingroup cmpnodes
 */

#include "node_composite_util.hh"

namespace blender::nodes {

static void cmp_node_sunbeams_declare(NodeDeclarationBuilder &b)
{
  b.add_input<decl::Color>(N_("Image")).default_value({1.0f, 1.0f, 1.0f, 1.0f});
  b.add_output<decl::Color>(N_("Image"));
}

}  // namespace blender::nodes

static void init(bNodeTree *UNUSED(ntree), bNode *node)
{
  NodeSunBeams *data = (NodeSunBeams *)MEM_callocN(sizeof(NodeSunBeams), "sun beams node");

  data->source[0] = 0.5f;
  data->source[1] = 0.5f;
  node->storage = data;
}

void register_node_type_cmp_sunbeams(void)
{
  static bNodeType ntype;

  cmp_node_type_base(&ntype, CMP_NODE_SUNBEAMS, "Sun Beams", NODE_CLASS_OP_FILTER, 0);
  ntype.declare = blender::nodes::cmp_node_sunbeams_declare;
  node_type_init(&ntype, init);
  node_type_storage(
      &ntype, "NodeSunBeams", node_free_standard_storage, node_copy_standard_storage);

  nodeRegisterType(&ntype);
}
