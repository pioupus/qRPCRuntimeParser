#include "rpc_ui.h"

#include "rpcruntime_function.h"
#include "rpcruntime_paramter_description.h"

void fill_array_child(QTreeWidgetItem *item, const RPCRuntimeDecodedParam &param);
void fill_character_child(QTreeWidgetItem *item, const RPCRuntimeDecodedParam &param);
void fill_enumeration_child(QTreeWidgetItem *item, const RPCRuntimeDecodedParam &param);
void fill_integer_child(QTreeWidgetItem *item, const RPCRuntimeDecodedParam &param);
void fill_structure_child(QTreeWidgetItem *item, const RPCRuntimeDecodedParam &param);
void fill_child(QTreeWidgetItem *item, const RPCRuntimeDecodedParam &param);

QTreeWidgetItem *get_last_child(QTreeWidgetItem *item) {
	return item->child(item->childCount() - 1);
}

QString to_hex_view(int64_t integer, int octets) {
	auto str = QString::number(integer, 16).toUpper();
	while (str.size() < octets) {
		str = "0" + str;
	}
	return "0x" + str;
}

void fill_array_child(QTreeWidgetItem *item, const RPCRuntimeDecodedParam &param) {
	item->addChild(new QTreeWidgetItem);
	auto array = get_last_child(item);
	array->setData(0, Qt::UserRole, "array");
	array->setData(1, Qt::UserRole, (param.get_desciption()->get_parameter_name() + ": " + param.get_desciption()->get_parameter_type()).c_str());

	array->setText(0, (param.get_desciption()->get_parameter_name() + "(" + param.get_desciption()->get_parameter_type() + ")").c_str());

	switch (param.get_desciption()->as_array().type.get_type()) {
		case RPCRuntimeParameterDescription::Type::integer: //value is a hex view of the integers
		{
			QString text;
			for (int i = 0; i < param.get_desciption()->as_array().number_of_elements; i++) {
				text += to_hex_view(param.as_array()[i].as_integer(), param.get_desciption()->as_array().type.get_bit_size() / 4);
				text += " ";
			}
			text.chop(1);
			array->setText(1, std::move(text));
		} break;
		case RPCRuntimeParameterDescription::Type::enumeration: //value is a hex view of the integer and the name if available
			break;
		case RPCRuntimeParameterDescription::Type::character: //value is a string
			break;
		case RPCRuntimeParameterDescription::Type::array:     //value is just empty, will need to go down a level
		case RPCRuntimeParameterDescription::Type::structure: //value is just empty, will need to go down a level
			for (int i = 0; i < param.get_desciption()->as_array().number_of_elements; i++) {
				fill_child(array, param.as_array()[i]);
				get_last_child(array)->setText(0, "[" + QString::number(i) + "]");
			}
			break;
	}
}

void fill_character_child(QTreeWidgetItem *item, const RPCRuntimeDecodedParam &param) {
	item->addChild(new QTreeWidgetItem);
	auto character = get_last_child(item);
	character->setData(0, Qt::UserRole, "character");
	character->setData(1, Qt::UserRole, (param.get_desciption()->get_parameter_name() + ": " + param.get_desciption()->get_parameter_type()).c_str());
}

void fill_enumeration_child(QTreeWidgetItem *item, const RPCRuntimeDecodedParam &param) {
	item->addChild(new QTreeWidgetItem);
	auto enumeration = get_last_child(item);
	enumeration->setData(0, Qt::UserRole, "enumeration");
	enumeration->setData(1, Qt::UserRole, (param.get_desciption()->get_parameter_name() + ": " + param.get_desciption()->get_parameter_type()).c_str());

	enumeration->setText(0, (param.get_desciption()->get_parameter_name() +  "(" + param.get_desciption()->get_parameter_type() + ")").c_str());
	enumeration->setText(1, param.as_enum().name.c_str());
}

void fill_integer_child(QTreeWidgetItem *item, const RPCRuntimeDecodedParam &param) {
	item->addChild(new QTreeWidgetItem);
	auto integer = get_last_child(item);
	integer->setData(0, Qt::UserRole, "enumeration");
	integer->setData(1, Qt::UserRole, (param.get_desciption()->get_parameter_name() + ": " + param.get_desciption()->get_parameter_type()).c_str());
	integer->setText(0, (param.get_desciption()->get_parameter_name() + "(" + param.get_desciption()->get_parameter_type() + ")").c_str());
	integer->setText(1, QString::number(param.as_integer()));
}

void fill_structure_child(QTreeWidgetItem *item, const RPCRuntimeDecodedParam &param) {
	item->addChild(new QTreeWidgetItem);
	auto structure = get_last_child(item);
	structure->setData(0, Qt::UserRole, "structure");
	structure->setData(1, Qt::UserRole, (param.get_desciption()->get_parameter_name() + ": " + param.get_desciption()->get_parameter_type()).c_str());
	for (auto &member : param.as_struct()) {
		fill_child(structure, member.type);
	}
}

void fill_child(QTreeWidgetItem *item, const RPCRuntimeDecodedParam &param) {
	switch (param.get_desciption()->get_type()) {
		case RPCRuntimeParameterDescription::Type::array:
			fill_array_child(item, param);
			break;
		case RPCRuntimeParameterDescription::Type::character:
			fill_character_child(item, param);
			break;
		case RPCRuntimeParameterDescription::Type::enumeration:
			fill_enumeration_child(item, param);
			break;
		case RPCRuntimeParameterDescription::Type::integer:
			fill_integer_child(item, param);
			break;
		case RPCRuntimeParameterDescription::Type::structure:
			fill_structure_child(item, param);
			break;
	}
}

std::unique_ptr<QTreeWidgetItem> getTreeWidgetReport(const RPCRuntimeDecodedFunctionCall &function) {
	auto retval = std::make_unique<QTreeWidgetItem>();
	retval->setText(0, function.get_declaration()->get_function_name().c_str());
	retval->setText(1, function.get_declaration()->get_function_declaration().c_str());

	retval->addChild(new QTreeWidgetItem);
	auto message_child = get_last_child(retval.get());
	message_child->setText(0, function.is_request() ? "Request" : "Reply");

	for (auto &param : function.get_decoded_parameters()) {
		fill_child(message_child, param);
	}
	return retval;
}

void add_to_report(QTreeWidgetItem *item, QStringList &list, int depth = 0) {
	QString line{depth, '\t'};

	//This is horrible code to get the right whitespaces at the end of the right lines.
	//We should just drop the whitespace requirements instead of implementing them.

	enum {named_array, unnamed_array, structure, other} our_type = other;

	auto type = item->text(0);
	if (type.contains('[')){ //could be array or struct
		if (item->text(0).contains("(")){
			our_type = named_array;
		}
		else{
			our_type = unnamed_array;
		}
	}

	switch (our_type) {
	case named_array:
		line += item->text(0) + ":\t";
		if (!item->text(1).isEmpty()){
			list.append(std::move(line));
			line = QString{depth + 1, '\t'};
			line += item->text(1) + " ";
		}
		break;
	case unnamed_array:
		line += item->text(0) + ":";
		if (!item->text(1).isEmpty()){
			list.append(std::move(line));
			line = QString{depth + 1, '\t'};
			line += item->text(1) + " ";
		}
		break;
	case structure:
		line += item->text(0) + ":\t";
		break;
	case other:
		line += item->text(0) + ":\t" + item->text(1) + " ";
	}
	list.append(std::move(line));
	for (int index = 0; index < item->childCount(); index++) { //stupid Qt doesn't support range based for loops
		add_to_report(item->child(index), list, depth + 1);
	}
}

QStringList get_report(const RPCRuntimeDecodedFunctionCall &function) {
	QStringList retval;
	auto tree = getTreeWidgetReport(function);
	auto func = tree->child(0);
	retval.append(func->text(0) + ": " + tree->text(0));
	retval.append("Function: " + tree->text(1));
	retval.append("");
	add_to_report(func->child(0), retval);
	return retval;
}
