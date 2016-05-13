#ifndef UPDATEMAN_H
#define UPDATEMAN_H

#include <list>
#include "cmap.h"
#include "cunit.h"

typedef std::list<CUnit*> unitlist;

class UpdateMan
{
	static UpdateMan* ms_instance;

private:
	UpdateMan(const UpdateMan& rhs);
	UpdateMan& operator=(const UpdateMan& rhs);
	
	unitlist *units;
	unitlist *toadd; //buffer array for new units, emptied every update

public:
	static UpdateMan* Instance();
	static void Release();
	
	inline const unitlist* GetUnits() {
		return static_cast<const unitlist*>(units);
		}
	void UpdateUnits(float dt, CMap* map);
	CUnit* GetUnit(int id);
	bool IsAlive(int id);
	inline void AddUnit(CUnit* unit) {
			toadd->push_back(unit);
	}
	inline void ClearUnits() {
		toadd->clear();
		units->clear();
	}
private:
	UpdateMan();
	~UpdateMan();

};

#endif // UPDATEMAN_H
