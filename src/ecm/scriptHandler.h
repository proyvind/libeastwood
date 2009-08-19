/* DST  -  Dune 2 Script Tools
 *  
 * Copyright (C) 2009 segra		<segra@strobs.com>

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *  
 * 
 * $Id: scriptHandler.h 21 2009-04-11 01:11:32Z segra $
 * 
 */

#include "../StdDef.h"
#include <string>
#include <vector>

class _scriptHandler;

// Opcode Definitions
typedef void (_scriptHandler::*opcodefuncPtr)();

struct _Opcode {
    const char *description;
    opcodefuncPtr function;
};

struct labelPosition {
    uint16_t _scriptPos;
    std::string _name;
};


enum _scriptTypes {
    _scriptBUILD = 0,
    _scriptUNIT,
    _scriptHOUSE
};

// Endian functions
// Read a uint16_t from the buffer
static inline
uint16_t readWord( const uint16_t *buffer ) {
    const uint8_t *uint16_tByte = (const uint8_t *) buffer;
    return (uint16_tByte[0] << 8) + uint16_tByte[1];
}

class _scriptHandler {
    public:
	_scriptHandler(const char *fileName);
	~_scriptHandler();

	uint16_t	scriptOpcodeFind(std::string opcodeStr, const _Opcode *opcodes);	// Search the opcode table for 'Opcode' string

	inline
	size_t		labelCountGet() {
	    return _lineCount;
	}

	// Virtual Functions
	virtual bool	 execute()		= 0;

    protected:
	// Opcode Prepare
	virtual void	 opcodesSetup();
	virtual void	 opcodesBuildingsSetup();
	virtual void	 opcodesUnitsSetup();
	virtual void	 opcodesHousesSetup();

	// Opcodes
	virtual void	 o_goto()		= 0;					
	virtual void	 o_setreturn()		= 0;
	virtual void	 o_pushOp()		= 0;
	virtual void	 o_push	() 		= 0;
	virtual void	 o_pushWord()		= 0;
	virtual void	 o_pushreg()		= 0;
	virtual void	 o_pushframeMinArg()	= 0;
	virtual void	 o_pushframePluArg()	= 0;
	virtual void	 o_popret()		= 0;
	virtual void	 o_popreg()		= 0;
	virtual void	 o_popframeMinArg()	= 0;
	virtual void	 o_popframePluArg()	= 0;
	virtual void	 o_spadd()		= 0;
	virtual void	 o_spsub()		= 0;
	virtual void	 o_execute()		= 0;
	virtual void	 o_ifnotgoto()		= 0;
	virtual void	 o_negate()		= 0;
	virtual void	 o_evaluate()		= 0;
	virtual void	 o_return()		= 0;

	// Opcode Evaluation Modes
	virtual void	 o_evaluate_IfEither(){}
	virtual void	 o_evaluate_Equal(){}
	virtual void	 o_evaluate_NotEqual(){}
	virtual void	 o_evaluate_CompareGreaterEqual(){}
	virtual void	 o_evaluate_CompareGreater(){}
	virtual void	 o_evaluate_CompareLessEqual(){}
	virtual void	 o_evaluate_CompareLess(){}
	virtual void	 o_evaluate_Add(){}
	virtual void	 o_evaluate_Subtract(){}
	virtual void	 o_evaluate_Multiply(){}
	virtual void	 o_evaluate_Divide(){}
	virtual void	 o_evaluate_ShiftRight(){}
	virtual void	 o_evaluate_ShiftLeft(){}
	virtual void	 o_evaluate_And(){}
	virtual void	 o_evaluate_Or(){}
	virtual void	 o_evaluate_DivideRemainder(){}
	virtual void	 o_evaluate_XOR(){}

	// Opcode Execute Functions
	// Buildings
	virtual void	 o_execute_Building_Null(){}

	// Units
	virtual void	 o_execute_Unit_Null(){}
	virtual void	 o_execute_Unit_GetDetail(){}

	// Houses
	virtual void	 o_execute_House_Null(){}

	// Opcode Functions
	const _Opcode	*_opcodes;		// Script Opcodes
	const _Opcode	*_opcodesEvaluate;	// Evaluate Command Opcodes
	const _Opcode	*_opcodesExecute;	// Execute Functions


	const char	*_fileName;		// Open File
	uint16_t	*_headerPointers;	// function pointers

	uint16_t	_pointerCount;		// Number of script "function" pointers
	size_t		_scriptSize;		// Size of script

	volatile bool	_modePreProcess;	// In Pre-Process mode?

	const char	**_objectNames;		// Pointer to current script object names
	const char	**_objectFunctions;	// Pointer to current script executable functions

	size_t		_lineCount;				
	const uint8_t	*_scriptBuffer;		// script uint8_t stream
	uint16_t	*_scriptPtr;		// Pointer in _scriptBuffer to current opcode
	uint16_t	_scriptPos;		// Line number of current opcode
	const uint8_t	*_scriptStart;		// pointer in _scriptBuffer to start of actual script
	_scriptTypes	_scriptType;		// Type of script (BUILD/TEAM/UNIT)

	std::vector<labelPosition> _scriptLabels;	// List of memory locations which can/are jumped to

	inline
	size_t scriptLabelGet(std::string label) {
	    size_t pos = scriptLabel(label);

	    if(pos == (size_t)-1)
		return -1;

	    return _scriptLabels[pos]._scriptPos;
	}

	inline
	size_t scriptLabel(std::string label) {
	    size_t pos = 0;

	    for(std::vector<labelPosition>::const_iterator labelIT = _scriptLabels.begin();
		    labelIT != _scriptLabels.end();
		    labelIT++, pos++) {

		if(label.compare((*labelIT)._name) == 0)
		    return pos;
	    }

	    // No labels here
	    return -1;
	}

	inline
	size_t scriptLabel(uint16_t position) {
	    size_t pos = 0;

	    for(std::vector<labelPosition>::const_iterator labelIT = _scriptLabels.begin();
		    labelIT != _scriptLabels.end();
		    labelIT++, pos++) {
		if((*labelIT)._scriptPos == position)
		    return pos;
	    }

	    // No labels here
	    return -1;
	}

	inline
	void scriptLabelAdd(std::string label, uint16_t position) {
	    labelPosition LP;
	    size_t labelPos = scriptLabel(position),
		   labelEndPos = label.find(":");

	    if(labelEndPos == std::string::npos)
		labelEndPos = label.length();

	    if(labelPos == (size_t)-1) {
		LP._name = label.substr(0, labelEndPos);
		LP._scriptPos = position;

		_scriptLabels.push_back(LP);
	    }
	}
};
