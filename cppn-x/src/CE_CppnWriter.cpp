/*
 * CE_CppnWriter.cpp
 *
 *  Created on: Jun 2, 2013
 *      Author: joost
 */

#include "CE_CppnWriter.h"
#include <iostream>
#include <iomanip>
#include "CE_Xml.h"
#include "CE_Util.h"

const std::string CppnWriter::version = "1.0";

#define writeEach(template_str,count,writer) open(template_str, count); for(size_t i=0; i<count; i++){writer;} close(template_str);


std::string indent(size_t nr){
	return std::string (nr*4, ' ');
}


template <typename Param1, typename Param2, typename Param3, typename Param4>
void CppnWriter::open(std::string template_str, Param1 param1, Param2 param2, Param3 param3, Param4 param4){
	std::ostringstream param1_stream;
	std::ostringstream param2_stream;
	std::ostringstream param3_stream;
	std::ostringstream param4_stream;
	param1_stream << std::setprecision(16) << param1;
	param2_stream << std::setprecision(16) << param2;
	param3_stream << std::setprecision(16) << param3;
	param4_stream << std::setprecision(16) << param4;

	output << indent(indentCt) + ce_xml::getOpenXmlString(template_str, param1_stream.str(), param2_stream.str(), param3_stream.str(), param4_stream.str()) << "\n";
	indentCt++;
}

//void CppnWriter::open(std::string template_str, std::string param1, std::string param2, std::string param3, std::string param4){
//	output << indent(indentCt) + ce_xml::getOpenXmlString(template_str, param1, param2, param3, param4) << "\n";
//	indentCt++;
//}
//
//void CppnWriter::open(std::string template_str, size_t param1){
//	output << indent(indentCt) + ce_xml::getOpenXmlString(template_str, param1) << "\n";
//	indentCt++;
//}

void CppnWriter::close(std::string template_str){
	indentCt--;
	output << indent(indentCt) + ce_xml::getCloseXmlString(template_str) << "\n";
}

//void CppnWriter::write(std::string template_str, std::string param1, std::string param2, std::string param3, std::string param4){
//	output << indent(indentCt) + ce_xml::getOneLineXmlString(template_str, param1, param2, param3, param4) << "\n";
//}

template <typename Param1, typename Param2, typename Param3, typename Param4>
void CppnWriter::write(std::string template_str, Param1 param1, Param2 param2, Param3 param3, Param4 param4){
	std::ostringstream param1_stream;
	std::ostringstream param2_stream;
	std::ostringstream param3_stream;
	std::ostringstream param4_stream;
	param1_stream << std::setprecision(16) << param1;
	param2_stream << std::setprecision(16) << param2;
	param3_stream << std::setprecision(16) << param3;
	param4_stream << std::setprecision(16) << param4;

	output << indent(indentCt) + ce_xml::getOneLineXmlString(template_str, param1_stream.str(), param2_stream.str(), param3_stream.str(), param4_stream.str()) << "\n";
}

//void CppnWriter::openClose(std::string template_str, std::string param){
//	output << indent(indentCt) + ce_xml::getOpenAndCloseXmlString(template_str, param) << "\n";
//}

template <typename ParamType>
void CppnWriter::openClose(std::string template_str, ParamType param){
	output << indent(indentCt) << ce_xml::getOpenXmlString(template_str) << param << ce_xml::getCloseXmlString(template_str) << "\n";
}


void CppnWriter::write(Cppn* cppn){
	output << ce_xml::getFirstLine() << "\n";
	write(ce_xml::cppn_data, version);
	open(ce_xml::data, cppn->getDataVersion());
	open(ce_xml::genomePhen, cppn->getAge(), cppn->getPhenotype());
	write(ce_xml::identifier, cppn->getBranch(), cppn->getId());

	writeEach(ce_xml::parent_count, cppn->getNrOfParents(), write(ce_xml::identifier, cppn->getParentBranch(i), cppn->getParentId(i)));
	writeEach(ce_xml::buttons_count, cppn->getNrOfColorButtons(), writeColorButton(cppn->getColorButton(i)););
	writeEach(ce_xml::nodes_count, cppn->getNrOfNodes(), writeNode(cppn->getNode(i)););
	writeEach(ce_xml::link_count, cppn->getNrOfEdges(), writeEdge(cppn->getEdge(i)););

	close(ce_xml::genomePhen);
	close(ce_xml::data);
}

void CppnWriter::writeNode(Node* node){
	open(ce_xml::iocolornode, node->getAffinity(), node->getBias(), node->getLabel(), node->getType());
	write(ce_xml::marking, node->getBranch(), node->getId());
	openClose(ce_xml::activation, node->getXmlActivationFunction());
	write(ce_xml::color, node->getColor().red(), node->getColor().green(), node->getColor().blue());
	write(ce_xml::position, node->pos().x(), node->pos().y());
	close(ce_xml::colornode);
}

void CppnWriter::writeEdge(Edge* edge){
	open(ce_xml::link);
	write(ce_xml::marking, edge->getBranch(), edge->getId());
	write(ce_xml::source,  edge->sourceNode()->getBranch(),  edge->sourceNode()->getId());
	write(ce_xml::target, edge->destNode()->getBranch(), edge->destNode()->getId());
	openClose(ce_xml::weight, util::toString(edge->getOriginalWeight()));
	write(ce_xml::color, edge->getColor().red(), edge->getColor().green(), edge->getColor().blue());
	close(ce_xml::link);
}

void CppnWriter::writeColorButton(CE_ColorButton* colorButton){
	open(ce_xml::color_button);
	openClose(ce_xml::text, colorButton->getText());
	write(ce_xml::color, colorButton->getColor().red(), colorButton->getColor().green(), colorButton->getColor().blue());
	close(ce_xml::color_button);
}