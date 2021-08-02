#pragma once

#include "pch.h"

/// <summary>
/// 语言类型。参见：Diablo II Language: https://d2mods.info/forum/viewtopic.php?t=19836
/// </summary>
enum class ELANGUAGE
{
    /// <summary>
    /// english
    /// </summary>
    ENG_DEFAULT = 0,
    /// <summary>
    /// spanish
    /// </summary>
    ESP,
    /// <summary>
    /// german
    /// </summary>
    DEU,
    /// <summary>
    /// french
    /// </summary>
    FRA,
    /// <summary>
    /// portuguese
    /// </summary>
    POR,
    /// <summary>
    /// italian
    /// </summary>
    ITA,
    /// <summary>
    /// japanese
    /// </summary>
    JPN,
    /// <summary>
    /// korean
    /// </summary>
    KOR,
    /// <summary>
    /// SIN
    /// </summary>
    SIN,
    /// <summary>
    /// chinese
    /// </summary>
    CHI,
    /// <summary>
    /// polish
    /// </summary>
    POL,
    /// <summary>
    /// russian
    /// </summary>
    RUS,
    /// <summary>
    /// english
    /// </summary>
    ENG
};

struct HD2ARCHIVE__
{
    void* example;
};