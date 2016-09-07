#include "rpcruntime_decoded_param.h"

RPCRuntimeDecodedParam::RPCRuntimeDecodedParam(RPCRuntimeParameterDescription paramDescription)
{
	this->paramDescription = paramDescription;
	value = 0;
	null = false;
}

RPCRuntimeDecodedParam::RPCRuntimeDecodedParam()
{
	value = 0;
   // paramDescription = NULL;
	null = true;
}

QByteArray RPCRuntimeDecodedParam::decode(QByteArray inBuffer)
{
	value = 0;
	uint64_t val=0;
	int lengthInByte = paramDescription.elementBitLength/8;
	uint64_t signBitPosition = paramDescription.elementBitLength-1;
	signBitPosition = 1 << signBitPosition;

	if (paramDescription.rpcParamType == RPCParamType_t::param_struct){
		lengthInByte = 0;
	}else{


		for(int i=0;i<lengthInByte; i++){
			uint8_t byte = inBuffer[i];
			val += ((uint64_t)byte * (1 << (8*i)));

		}

		if (paramDescription.rpcParamType == RPCParamType_t::param_int){

			if (paramDescription.isSigned ==false){
				if (lengthInByte==1)
					string = "0x"+QString("%1").arg(val, 2, 16, QChar('0')).toUpper();
				else
					string = QString::number(val);
				value = val;
			}else{
				uint64_t v = 0;
				if(val & signBitPosition){
					for(size_t i=(size_t)lengthInByte;i<sizeof(uint64_t); i++){
						v += ((uint64_t)0xFF * ((uint64_t)1 << 8*i));
					}
				}
				val += v;
				value = (int64_t)val;
				string = QString::number(value);
			}
		}else if(paramDescription.rpcParamType == RPCParamType_t::param_character){
			value = val;
		}else if(paramDescription.rpcParamType == RPCParamType_t::param_enum){
			value = val;
			QList<QPair<int,QString>> enumValues = paramDescription.enumValues;
			string = "illegal enum value";
			for(int i = 0; i<enumValues.count();i++){
				QPair<int,QString> item = enumValues[i];
				if (item.first == value){
					string = item.second;
					break;
				}
			}
		}


	}
	return inBuffer.mid(lengthInByte);
}

RPCRuntimeParameterDescription RPCRuntimeDecodedParam::getParamDescription() const
{
	return paramDescription;
}

bool RPCRuntimeDecodedParam::isNull()
{
	return null;
}
