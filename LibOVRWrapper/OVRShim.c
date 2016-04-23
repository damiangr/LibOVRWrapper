#include "stdafx.h"

#if !defined(OVR_DLL_BUILD)
	#define OVR_DLL_BUILD
#endif

#include "../LibOVR0.8/Include/OVR_CAPI_0_8_0.h"

#include "OVRShim.h"

OVR_PUBLIC_FUNCTION(ovrResult) ovr_Initialize(const ovrInitParams* params) {
	return ovr_Initialize1_3((ovrInitParams1_3*)params);
}

OVR_PUBLIC_FUNCTION(void) ovr_Shutdown() {
	ovr_Shutdown1_3();
}

OVR_PUBLIC_FUNCTION(void) ovr_GetLastErrorInfo(ovrErrorInfo* errorInfo) {
	ovr_GetLastErrorInfo1_3((ovrErrorInfo1_3*)errorInfo);
}

OVR_PUBLIC_FUNCTION(const char*) ovr_GetVersionString() {
	return "OculusSDK0.8";
}

OVR_PUBLIC_FUNCTION(int) ovr_TraceMessage(int level, const char* message) {
	return ovr_TraceMessage1_3(level, message);
}

OVR_PUBLIC_FUNCTION(ovrHmdDesc) ovr_GetHmdDesc(ovrSession session) {
	ovrHmdDesc1_3 desc = ovr_GetHmdDesc1_3((ovrSession1_3)session);

	ovrHmdDesc d;

	d.AvailableHmdCaps = desc.AvailableHmdCaps;
	d.AvailableTrackingCaps = desc.AvailableTrackingCaps;
	
	ovrTrackerDesc1_3 tracker = ovr_GetTrackerDesc1_3((ovrSession1_3)session, 0);

	d.CameraFrustumFarZInMeters = tracker.FrustumFarZInMeters;
	d.CameraFrustumHFovInRadians = tracker.FrustumHFovInRadians;
	d.CameraFrustumNearZInMeters = tracker.FrustumNearZInMeters;
	d.CameraFrustumVFovInRadians = tracker.FrustumVFovInRadians;
	
	memcpy(d.DefaultEyeFov, desc.DefaultEyeFov, sizeof(d.DefaultEyeFov));
	d.DefaultHmdCaps = desc.DefaultHmdCaps;
	d.DefaultTrackingCaps = desc.DefaultTrackingCaps;
	d.DisplayRefreshRate = desc.DisplayRefreshRate;
	d.FirmwareMajor = desc.FirmwareMajor;
	d.FirmwareMinor = desc.FirmwareMinor;
	strncpy_s(d.Manufacturer, sizeof(d.Manufacturer), desc.Manufacturer, sizeof(d.Manufacturer) / sizeof(d.Manufacturer[0]));
	memcpy(d.MaxEyeFov, desc.MaxEyeFov, sizeof(d.MaxEyeFov));
	
	d.ProductId = desc.ProductId;
	strncpy_s(d.ProductName, sizeof(d.ProductName), desc.ProductName, sizeof(d.ProductName) / sizeof(d.ProductName[0]));
	d.Resolution = desc.Resolution;
	
	strncpy_s(d.SerialNumber, sizeof(d.SerialNumber), desc.SerialNumber, sizeof(d.SerialNumber) / sizeof(d.SerialNumber[0]));
	d.VendorId = desc.VendorId;

	if (desc.Type > 12) {
		d.Type = 12;
	}
	else {
		d.Type = desc.Type;
	}

	return d;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_Create(ovrSession* pSession, ovrGraphicsLuid* pLuid) {
	return ovr_Create1_3((ovrSession1_3*)pSession, (ovrGraphicsLuid1_3*)pLuid);
}

OVR_PUBLIC_FUNCTION(void) ovr_Destroy(ovrSession session) {
	ovr_Destroy1_3((ovrSession1_3)session);
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_GetSessionStatus(ovrSession session, ovrSessionStatus* sessionStatus) {
	ovrSessionStatus1_3 status;

	ovrResult r = ovr_GetSessionStatus1_3((ovrSession1_3)session, &status);

	sessionStatus->HmdPresent = status.HmdPresent;
	sessionStatus->HasVrFocus = status.IsVisible;

	if (status.ShouldRecenter) {
		ovr_RecenterTrackingOrigin1_3((ovrSession1_3)session);

		//or ovr_ClearShouldRecenterFlag?
	}

	return r;
}

OVR_PUBLIC_FUNCTION(unsigned int) ovr_GetEnabledCaps(ovrSession session) {
	ovrHmdDesc1_3 desc = ovr_GetHmdDesc1_3((ovrSession1_3)session);

	//not possible anymore
	return desc.DefaultHmdCaps;
}

OVR_PUBLIC_FUNCTION(void) ovr_SetEnabledCaps(ovrSession session, unsigned int hmdCaps) {
	//not possible anymore
}

OVR_PUBLIC_FUNCTION(unsigned int) ovr_GetTrackingCaps(ovrSession session) {
	ovrHmdDesc1_3 desc = ovr_GetHmdDesc1_3((ovrSession1_3)session);

	return desc.DefaultTrackingCaps;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_ConfigureTracking(ovrSession session, unsigned int requestedTrackingCaps,
	unsigned int requiredTrackingCaps) {
	//not used anymore
	return ovrSuccess1_3;
}

OVR_PUBLIC_FUNCTION(void) ovr_RecenterPose(ovrSession session) {
	ovr_RecenterTrackingOrigin1_3((ovrSession1_3)session);
}

void copyPose(ovrPosef* dest, ovrPosef1_3* source) {
	dest->Orientation = source->Orientation;
	dest->Position = source->Position;
}

void copyPoseState(ovrPoseStatef* dest, ovrPoseStatef1_3* source) {
	dest->AngularAcceleration = source->AngularAcceleration;
	dest->AngularVelocity = source->AngularVelocity;
	dest->LinearAcceleration = source->LinearAcceleration;
	dest->LinearVelocity = source->LinearVelocity;
	copyPose(&(dest->ThePose), &(source->ThePose));
	dest->TimeInSeconds = source->TimeInSeconds;
}

OVR_PUBLIC_FUNCTION(ovrTrackingState) ovr_GetTrackingState(ovrSession session, double absTime, ovrBool latencyMarker) {
	ovrTrackingState1_3 state = ovr_GetTrackingState1_3((ovrSession1_3)session, absTime, latencyMarker);
	ovrTrackerPose1_3 tpose = ovr_GetTrackerPose1_3((ovrSession1_3)session, 0);

	ovrTrackingState r;	
	copyPose(&(r.CameraPose), &(tpose.Pose));
	r.CameraPose.Orientation = tpose.Pose.Orientation;
	r.CameraPose.Position = tpose.Pose.Position;
	copyPoseState(&(r.HandPoses[0]), &(state.HandPoses[0]));
	copyPoseState(&(r.HandPoses[1]), &(state.HandPoses[1]));

	r.HandStatusFlags[0] = state.HandStatusFlags[0];
	r.HandStatusFlags[1] = state.HandStatusFlags[1];

	copyPose(&(r.HeadPose), &(state.HeadPose));

	//r.LastCameraFrameCounter not filled

	copyPose(&(r.LeveledCameraPose), &(tpose.LeveledPose));

	//r.RawSensorData not filled
	r.StatusFlags = state.StatusFlags | ovrStatus_CameraPoseTracked | ovrStatus_PositionConnected | ovrStatus_HmdConnected;
	
	return r;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_GetInputState(ovrSession session, unsigned int controllerTypeMask, ovrInputState* inputState) {
	ovrInputState1_3 state;

	ovrResult res = ovr_GetInputState1_3((ovrSession1_3)session, (ovrControllerType1_3)controllerTypeMask, &state);

	if (res < 0) {
		return res;
	}

	inputState->Buttons = state.Buttons; // needs mapping?
	inputState->ConnectedControllerTypes = ovr_GetConnectedControllerTypes1_3((ovrSession1_3)session);
	inputState->HandTrigger[0] = state.HandTrigger[0];
	inputState->HandTrigger[1] = state.HandTrigger[1];

	inputState->IndexTrigger[0] = state.IndexTrigger[0];
	inputState->IndexTrigger[1] = state.IndexTrigger[1];

	inputState->Thumbstick[0] = state.Thumbstick[0];
	inputState->Thumbstick[1] = state.Thumbstick[1];

	inputState->TimeInSeconds = state.TimeInSeconds;
	inputState->Touches = state.Touches;

	return res;
}