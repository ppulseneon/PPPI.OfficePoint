/* -- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 - - /

This file is part of the LibreOffice project.
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/ .
This file incorporates work covered by the following license notice:
Licensed to the Apache Software Foundation (ASF) under one or more
contributor license agreements. See the NOTICE file distributed
with this work for additional information regarding copyright
ownership. The ASF licenses this file to you under the Apache
License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of
the License at http://www.apache.org/licenses/LICENSE-2.0 .
*/

#include <tools/gen.hxx>

/**
 * @brief Поворачивает точку вокруг другой точки на заданный угол.
 *
 * Эта функция поворачивает заданную точку `rPoint` вокруг текущей точки (`this`)
 * на угол, указанный в параметре `nOrientation`. Поворот выполняется "на месте",
 * то есть координаты точки `rPoint` изменяются напрямую.
 *
 * @param rPoint Ссылка на точку, которую нужно повернуть.
 * @param nOrientation Угол поворота в десятых долях градуса.
 */
void PointTemplateBase::RotateAround(PointTemplateBase& rPoint,
                                     Degree10 nOrientation) const
{
    tools::Long nX = rPoint.mnA;
    tools::Long nY = rPoint.mnB;
    RotateAround(nX, nY, nOrientation);
    rPoint.mnA = nX;
    rPoint.mnB = nY;
}

/**
 * @brief Поворачивает точку вокруг начала координат на заданный угол.
 *
 * Эта функция поворачивает точку с координатами `(rX, rY)` вокруг начала координат,
 * заданного текущей точкой (`this`), на угол, указанный в параметре `nOrientation`.
 * Поворот выполняется "на месте", то есть значения `rX` и `rY` изменяются напрямую.
 *
 * @param rX Ссылка на X-координату точки, которую нужно повернуть.
 * @param rY Ссылка на Y-координату точки, которую нужно повернуть.
 * @param nOrientation Угол поворота в десятых долях градуса.
 */
void PointTemplateBase::RotateAround(tools::Long& rX, tools::Long& rY,
                                     Degree10 nOrientation) const
{
    const tools::Long nOriginX = mnA; // X-координата центра поворота.
    const tools::Long nOriginY = mnB; // Y-координата центра поворота.

    if ((nOrientation >= 0_deg10) && !(nOrientation % 900_deg10))
    {
        if (nOrientation >= 3600_deg10)
            nOrientation %= 3600_deg10;

        if (nOrientation)
        {
            // Переносим точку так, чтобы центр поворота стал началом координат.
            rX -= nOriginX;
            rY -= nOriginY;

            // Выполняем поворот в зависимости от угла.
            if (nOrientation == 900_deg10)
            {
                tools::Long nTemp = rX;
                rX = rY;
                rY = -nTemp;
            }
            else if (nOrientation == 1800_deg10)
            {
                rX = -rX;
                rY = -rY;
            }
            else /* ( nOrientation == 2700 ) */
            {
                tools::Long nTemp = rX;
                rX = -rY;
                rY = nTemp;
            }

            // Возвращаем точку обратно к исходному положению.
            rX += nOriginX;
            rY += nOriginY;
        }
    }
    else
    {
        // Преобразуем угол из градусов в радианы.
        double nRealOrientation = toRadians(nOrientation);
        double nCos = cos(nRealOrientation); // Косинус угла поворота.
        double nSin = sin(nRealOrientation); // Синус угла поворота.

        // Переносим точку так, чтобы центр поворота стал началом координат.
        tools::Long nX = rX - nOriginX;
        tools::Long nY = rY - nOriginY;

        // Выполняем поворот с использованием тригонометрических функций.
        rX = +static_cast<tools::Long>(nCos * nX + nSin * nY) + nOriginX;
        rY = -static_cast<tools::Long>(nSin * nX - nCos * nY) + nOriginY;
    }
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */