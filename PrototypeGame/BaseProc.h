#pragma once

class BaseProc
{
public:
	BaseProc() = default;
	virtual ~BaseProc() = default;
	BaseProc(const BaseProc& other) = delete;
	BaseProc(BaseProc&& other) = delete;
	void operator=(const BaseProc& rhs) = delete;
	void operator=(const BaseProc&& rhs) = delete;

	virtual bool IsProcActive() = 0;
	virtual void Trigger() = 0;
private:
};