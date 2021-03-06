#ifndef DIABLO_EXE_H
#define DIABLO_EXE_H

#include <string>
#include <map>
#include <vector>

#include <settings/settings.h>
#include <faio/faio.h>

#include "monster.h"
#include "npc.h"
#include "baseitem.h"
#include "prefix.h"
#include "uniqueitem.h"

namespace DiabloExe
{
    class DiabloExe
    {
        public:
            DiabloExe();

            const Monster& getMonster(const std::string& name) const;
            std::vector<const Monster*> getMonstersInLevel(size_t levelNum) const;

            const Npc& getNpc(const std::string& name) const;
            std::vector<const Npc*> getNpcs() const;

            const BaseItem& getItem(const std::string& name) const;
            std::map<std::string, BaseItem> getItemMap() const;

            std::string dump() const;

            bool isLoaded() const;

        private:
            std::string getMD5();
            std::string getVersion();

            void loadMonsters(FAIO::FAFile* exe);
            void loadNpcs(FAIO::FAFile* exe);
            void loadBaseItems(FAIO::FAFile* exe);
            void loadUniqueItems(FAIO::FAFile* exe);
            void loadPreficies(FAIO::FAFile* exe);

            Settings::Settings mSettings;
            std::string mVersion;
            std::map<std::string, Monster> mMonsters;
            std::map<std::string, Npc> mNpcs;
            std::map<std::string, BaseItem> mBaseItems;
            std::map<std::string, UniqueItem> mUniqueItems;
            std::map<std::string, Prefix> mPrefices;
    };
}

#endif
