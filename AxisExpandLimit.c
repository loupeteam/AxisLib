
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "AxisLib.h"
#ifdef __cplusplus
	};
#endif
/*	Temporarily set the limits of an axis 
	This function block is useful when an 
	axis is controlled using MC_BR_MoveCyclicPos
	and the axis is outside it's axis limits	*/
void AxisExpandLimit(struct AxisExpandLimit* inst)
{

	if(inst == 0) return;
	if(inst->Axis == 0) {
		inst->Busy= 0;
		inst->Error= 1;
		inst->ErrorID= 29200; 
		return;
	}

	ACP10AXIS_typ *pAxis = (ACP10AXIS_typ*) inst->Axis;

	if(!inst->AcknowledgeError){
		inst->Internal._AcknowledgeError= 0;	
	}

	if( (inst->AcknowledgeError && !inst->Internal._AcknowledgeError) || inst->Enable == inst->Active)
	{
		inst->Internal._AcknowledgeError= 1;
		inst->Error=	0;
		inst->ErrorID=	0;
	}
		
	switch(inst->Internal.State){
		//Idle - Default limits active
		case 0:
			inst->Active= 0; 
			inst->Busy= 0;
			if(inst->Enable && !inst->Error){
				inst->Internal.State++;
			}
			else{				
				break;
			}

		//Setting limits wider
		case 1:
			inst->Busy= 1;
			if(inst->Internal._MC_BR_InitAxisSubjectPar.Done){
				inst->Internal.State++;		
			}
			else if(inst->Internal._MC_BR_InitAxisSubjectPar.Error){
				inst->ErrorID= inst->Internal._MC_BR_InitAxisSubjectPar.ErrorID;
				inst->Error= 1; 
				inst->Internal.State--;
				//If error repopulate the application limits with the current active limits
				pAxis->limit.parameter.pos_sw_end= inst->Internal.ApplicationLimits.pos_sw_end;
				pAxis->limit.parameter.neg_sw_end= inst->Internal.ApplicationLimits.neg_sw_end;			
				break;
			}
			else if(!inst->Internal._MC_BR_InitAxisSubjectPar.Busy){
				inst->Internal._MC_BR_InitAxisSubjectPar.Execute= 1;

			//save the old limits to put the back later
				inst->Internal.ApplicationLimits.pos_sw_end= pAxis->limit.parameter.pos_sw_end;
				inst->Internal.ApplicationLimits.neg_sw_end= pAxis->limit.parameter.neg_sw_end;

			//Calculate the new limits
				if(pAxis->limit.parameter.pos_sw_end <= pAxis->monitor.s){
					pAxis->limit.parameter.pos_sw_end = pAxis->monitor.s + inst->Tolerance;
				}			
				if(pAxis->limit.parameter.neg_sw_end >= pAxis->monitor.s){
					pAxis->limit.parameter.neg_sw_end = pAxis->monitor.s - inst->Tolerance;
				}
				inst->Internal.ExpandedLimits.pos_sw_end= pAxis->limit.parameter.pos_sw_end;
				inst->Internal.ExpandedLimits.neg_sw_end= pAxis->limit.parameter.neg_sw_end;
				break;
			}
			else{
				break;
			}
		
		//Limits are widened
		case 2:
			inst->Active= 1; 
			inst->Busy= 0;			
			if(!inst->Enable && !inst->Error){
				inst->Internal.State++;
			}
			else{				
				break;
			}
			
			break;

		//Set limits to defaults
		case 3:
			inst->Busy= 1;
			if(inst->Internal._MC_BR_InitAxisSubjectPar.Done){
				inst->Internal.State= 0;		
			}
			else if(inst->Internal._MC_BR_InitAxisSubjectPar.Error){
				inst->ErrorID= inst->Internal._MC_BR_InitAxisSubjectPar.ErrorID;
				inst->Error= 1; 
				inst->Internal.State--;
				//If error repopulate the application limits with the current active limits
				pAxis->limit.parameter.pos_sw_end= inst->Internal.ExpandedLimits.pos_sw_end;
				pAxis->limit.parameter.neg_sw_end= inst->Internal.ExpandedLimits.neg_sw_end;			
				break;
			}
			else if(!inst->Internal._MC_BR_InitAxisSubjectPar.Busy){
				inst->Internal._MC_BR_InitAxisSubjectPar.Execute= 1;			
				inst->Internal.ExpandedLimits.pos_sw_end= pAxis->limit.parameter.pos_sw_end;
				inst->Internal.ExpandedLimits.neg_sw_end= pAxis->limit.parameter.neg_sw_end;
				pAxis->limit.parameter.pos_sw_end= inst->Internal.ApplicationLimits.pos_sw_end;
				pAxis->limit.parameter.neg_sw_end= inst->Internal.ApplicationLimits.neg_sw_end;
				break;
			}
			else{
				break;
			}

		//Cleanup
		default:
			inst->Active= 0; 
			inst->Busy= 0;	
			inst->Internal.State= 0;					
			break;		
	}
	
	
	inst->Internal._MC_BR_InitAxisSubjectPar.Axis=		(UDINT) pAxis;
	inst->Internal._MC_BR_InitAxisSubjectPar.Subject=	ncLIMITS;
	MC_BR_InitAxisSubjectPar(&inst->Internal._MC_BR_InitAxisSubjectPar);	
	inst->Internal._MC_BR_InitAxisSubjectPar.Execute= 	0;
	
	
	
}
