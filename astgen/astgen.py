import json
import argparse
from io import StringIO
from dataclasses import dataclass
from typing import List, Any, Dict
import os


@dataclass
class Field:
    name: str
    type: str


@dataclass
class Node:
    name: str
    subnodes: List["Node"]
    is_enum: bool
    fields: List[Field]
    elements: List[str]


@dataclass
class PrinterSchema:
    template: List[str]
    declaration: List[str]
    definition: List[str]
    definition_enum: List[str]
    definition_enum_case: List[str]
    definition_empty: List[str]
    field: List[str]

    def __init__(self, data: Dict[str, List[str]]):
        self.template = data["template"]
        self.declaration = data["declaration"]
        self.definition = data["definition"]
        self.definition_enum = data["definition_enum"]
        self.definition_enum_case = data["definition_enum_case"]
        self.definition_empty = data["definition_empty"]
        self.field = data["field"]


@dataclass
class Schema:
    sys_includes: List[str]
    project_includes: List[str]
    namespace: str
    ast: List[Node]
    printer: PrinterSchema

    @staticmethod
    def _parse_enum(name: str, data: List[str]) -> Node:
        return Node(
            name=name,
            is_enum=True,
            subnodes=[],
            fields=[],
            elements=data
        )

    @staticmethod
    def _parse_struct(name: str, data: Dict[str, Any]) -> Node:
        subnodes = []
        fields = []
        for subname, body in data.items():
            if isinstance(body, dict):
                subnodes.append(Schema._parse_struct(subname, body))
            elif isinstance(body, list):
                subnodes.append(Schema._parse_enum(subname, body))
            elif isinstance(body, str):
                fields.append(Field(name=subname, type=body))
            else:
                raise Exception(f"Invalid node definition (`{name}`)")

        return Node(
            name=name,
            is_enum=False,
            subnodes=subnodes,
            fields=fields,
            elements=[]
        )

    @staticmethod
    def _parse_nodes(data: Dict[str, Any]) -> List[Node]:
        res = []
        for name, body in data.items():
            res.append(Schema._parse_struct(name, body))
        return res

    def __init__(self, data: Dict[str, Any]):
        self.sys_includes = data["sys_includes"]
        self.project_includes = data["project_includes"]
        self.namespace = data["namespace"]
        self.ast = Schema._parse_nodes(data["ast"])
        self.printer = PrinterSchema(data["printer"])


@dataclass
class Generator:
    out: StringIO
    schema: Schema
    indent: int
    equality_struct_stack: List[str]

    def __init__(self, schema: Schema):
        self.out = StringIO()
        self.schema = schema
        self.indent = 0
        self.equality_struct_stack = []

    def generate_header(self) -> str:
        self.out = StringIO()
        self._gen_header()
        return self.out.getvalue()

    def generate_source(self) -> str:
        self.out = StringIO()
        self._gen_source()
        return self.out.getvalue()

    def _indent(self):
        for i in range(0, self.indent):
            self.out.write("    ")

    def _gen_forward_decl(self, node: Node):
        self._indent()
        if node.is_enum:
            self.out.write(f"enum class {node.name};\n")
        else:
            self.out.write(f"struct {node.name};\n")

    def _gen_decl(self, node: Node):
        if node.is_enum:
            self._indent()
            self.out.write(f"enum class {node.name} {{\n")
            self.indent += 1
            for element in node.elements:
                self._indent()
                self.out.write(f"{element},\n")
            self.indent -= 1
            self._indent()
            self.out.write("};\n")
        else:
            self._indent()
            self.out.write(f"struct {node.name} {{\n")
            self.indent += 1
            for subnode in node.subnodes:
                self._gen_decl(subnode)
            for field in node.fields:
                self._indent()
                self.out.write(f"{field.type} {field.name};\n")
            self.indent -= 1
            self._indent()
            self.out.write("};\n")

    def _get_equality_current_typename(self) -> str:
        return "::".join(self.equality_struct_stack)

    def _gen_equality_signature(self, node: Node, not_equal: bool):
        typename = self._get_equality_current_typename()
        attr = ""
        if len(node.fields) == 0:
            attr = "[[maybe_unused]] "
        op = ""
        if not_equal:
            op = "!="
        else:
            op = "=="

        self._indent()
        self.out.write(f"bool operator{op}(\
{attr}const {typename}& l, \
{attr}const {typename}& r)")

    def _gen_equality_forward_decl(self, node: Node, not_equal: bool):
        if not node.is_enum:
            self.equality_struct_stack.append(node.name)
            self._gen_equality_signature(node, not_equal)
            self.out.write(";\n")
            for subnode in node.subnodes:
                self._gen_equality_forward_decl(subnode, not_equal)
            self.equality_struct_stack.pop()

    def _gen_equality_decl(self, node: Node, not_equal: bool):
        if not node.is_enum:
            op = ""
            if not_equal:
                op = "!="
            else:
                op = "=="

            self.equality_struct_stack.append(node.name)
            self._gen_equality_signature(node, not_equal)
            self.out.write(" {\n")
            self.indent += 1

            self._indent()
            self.out.write("return ")

            if len(node.fields) == 0:
                self.out.write("true")
            else:
                for i in range(0, len(node.fields)):
                    field = node.fields[i]
                    self.out.write(f"l.{field.name} {op} r.{field.name}")
                    if i < len(node.fields) - 1:
                        self.out.write(" && ")

            self.out.write(";\n")

            self.indent -= 1
            self._indent()
            self.out.write("}\n")

            for subnode in node.subnodes:
                self._gen_equality_decl(subnode, not_equal)

            self.equality_struct_stack.pop()

    @dataclass
    class _PrinterNodeInfo:
        type: str
        name: str
        is_enum: bool
        fields: List[str]
        elements: List[str]

    def _compile_printer_field(self, field_name: str) -> List[str]:
        res = []
        for line in self.schema.printer.field:
            res.append(line.replace("$field_name", field_name))
        return res

    def _compile_printer_declaration(
        self,
        info: _PrinterNodeInfo
    ) -> List[str]:
        res = []
        for line in self.schema.printer.declaration:
            res.append(line.replace("$type", info.type))
        return res

    def _compile_printer_definition_enum_case(
        self,
        type: str,
        name: str,
        variant: str
    ) -> List[str]:
        res = []
        for line in self.schema.printer.definition_enum_case:
            line = line.replace("$type", type)
            line = line.replace("$name", name)
            res.append(line.replace("$variant", variant))
        return res

    def _compile_printer_definition(self, info: _PrinterNodeInfo) -> List[str]:
        if info.is_enum:
            subst = []
            for line in self.schema.printer.definition_enum:
                line = line.replace("$type", info.type)
                line = line.replace("$name", info.name)
                subst.append(line)

            cases = []
            for variant in info.elements:
                cases.extend(self._compile_printer_definition_enum_case(
                    info.type,
                    info.name,
                    variant
                ))

            res = []
            for line in subst:
                if line.lstrip().rstrip() == "#definition_enum_cases":
                    res.extend(cases)
                else:
                    res.append(line)

            return res
        else:
            if len(info.fields) == 0:
                res = []
                for line in self.schema.printer.definition_empty:
                    line = line.replace("$type", info.type)
                    line = line.replace("$name", info.name)
                    res.append(line)

                return res
            else:
                subst = []
                for line in self.schema.printer.definition:
                    line = line.replace("$type", info.type)
                    line = line.replace("$name", info.name)
                    subst.append(line)

                fields = []
                for field_name in info.fields:
                    fields.extend(self._compile_printer_field(field_name))

                res = []
                for line in subst:
                    if line.lstrip().rstrip() == "#fields":
                        res.extend(fields)
                    else:
                        res.append(line)

                return res

    def _compile_printer_template(
        self,
        infos: List[_PrinterNodeInfo]
    ) -> List[str]:
        declarations = []
        definitions = []
        for info in infos:
            declarations.extend(self._compile_printer_declaration(info))
            definitions.extend(self._compile_printer_definition(info))

        res = []
        for line in self.schema.printer.template:
            if line.lstrip().rstrip() == "#declarations":
                res.extend(declarations)
            elif line.lstrip().rstrip() == "#definitions":
                res.extend(definitions)
            else:
                res.append(line)

        return res

    @staticmethod
    def _get_node_infos(
        node: Node,
        node_infos: List[_PrinterNodeInfo],
        typename_stack: List[str]
    ):
        typename_stack.append(node.name)

        name = node.name
        type = "::".join(typename_stack)
        is_enum = node.is_enum
        fields = []
        elements = node.elements
        if not node.is_enum:
            for field in node.fields:
                fields.append(field.name)

        node_infos.append(Generator._PrinterNodeInfo(
            name=name,
            type=type,
            is_enum=is_enum,
            fields=fields,
            elements=elements
        ))

        for subnode in node.subnodes:
            Generator._get_node_infos(subnode, node_infos, typename_stack)

        typename_stack.pop()

    def _gen_printer(self):
        node_infos = []
        typename_stack = []
        for node in self.schema.ast:
            Generator._get_node_infos(node, node_infos, typename_stack)

        lines = self._compile_printer_template(node_infos)
        for line in lines:
            self._indent()
            self.out.write(line)
            self.out.write("\n")

    def _gen_heading(self):
        self.out.write("// This file is generated using `astgen.py`.    /\n")
        self.out.write("// DO NOT EDIT BY HAND!                         /\n")
        self.out.write("//                                              /\n")
        self.out.write("// Note: any modifications to the AST should be /\n")
        self.out.write("// done by changing the AST schema file         /\n")
        self.out.write("// `ast.json` and regenerating the sources.     /\n")
        self.out.write("\n")

    def _gen_header(self):
        self._gen_heading()

        self.out.write("#pragma once\n\n")

        for i in self.schema.sys_includes:
            self.out.write(f"#include <{i}>\n")
        self.out.write("\n")

        for i in self.schema.project_includes:
            self.out.write(f"#include \"{i}\"\n")
        self.out.write("\n")

        self.out.write(f"namespace {self.schema.namespace} {{\n\n")
        self.indent += 1

        # Forward declarations for top-level nodes
        for node in self.schema.ast:
            self._gen_forward_decl(node)

        # Actual definitions
        for node in self.schema.ast:
            self._gen_decl(node)

        # Forward declarations for equality operator
        for node in self.schema.ast:
            self._gen_equality_forward_decl(node, False)

        # Forward declarations for inequality operator
        for node in self.schema.ast:
            self._gen_equality_forward_decl(node, True)

        self._gen_printer()

        self.indent -= 1
        self.out.write("\n}\n")

    def _gen_source(self):
        self._gen_heading()

        self.out.write("#include \"ast.h\"\n\n")
        self.out.write(f"namespace {self.schema.namespace} {{\n\n")
        self.indent += 1

        # Definitions for equality operator
        for node in self.schema.ast:
            self._gen_equality_decl(node, False)

        # Definitions for inequality operator
        for node in self.schema.ast:
            self._gen_equality_decl(node, True)

        self.indent -= 1
        self.out.write("\n}\n")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="astgen",
        description="AST definition generator"
    )
    parser.add_argument(
        "-s", "--schema_file",
        required=True,
        help="AST definition file path"
    )
    parser.add_argument(
        "--clang-format-path",
        required=False,
        default="clang-format",
        help="path to clang-format executable"
    )
    parser.add_argument(
        "-c", "--clang-format-config",
        required=False,
        default=None,
        help="clang-format configuration file path. \
        Generated files will not be formated if this option is not set."
    )
    parser.add_argument(
        "-o", "--output-dir",
        required=True,
        default=None,
        help="output directory for generated files"
    )

    args = parser.parse_args()

    with open(args.schema_file, "r") as schema_file:
        raw = json.loads(schema_file.read())
        schema = Schema(raw)
        gen = Generator(schema)

        header = gen.generate_header()
        source = gen.generate_source()

        ast_h_path = args.output_dir + "/ast.h"
        ast_cpp_path = args.output_dir + "/ast.cpp"

        with open(ast_h_path, "w") as ast_h_file:
            ast_h_file.write(header)

        with open(ast_cpp_path, "w") as ast_cpp_file:
            ast_cpp_file.write(source)

        if args.clang_format_config is not None:
            for path in [ast_h_path, ast_cpp_path]:
                cmd = f"{args.clang_format_path} \
                    \"--style=file:{args.clang_format_config}\" \
                    -i \"{path}\""
                os.system(cmd)
