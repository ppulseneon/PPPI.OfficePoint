/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */

#include <sal/config.h>

#include <basegfx/numeric/ftools.hxx>
#include <tools/line.hxx>
#include <tools/helpers.hxx>

#include <cmath>

namespace tools
{

/**
 * @brief Вычисление длины линии.
 *
 * Рассчитывает длину линии, используя формулу Евклидова расстояния
 * между начальной и конечной точками.
 *
 * @return double Длина линии
 */
double Line::GetLength() const
{
    return hypot( maStart.X() - maEnd.X(), maStart.Y() - maEnd.Y() );
}

/**
 * @brief Поиск точки пересечения двух линий.
 *
 * Определяет, пересекаются ли две линии, и если да, то вычисляет
 * координаты точки пересечения в целочисленных координатах.
 *
 * @param rLine Вторая линия
 * @param[out] rIntersection Точка пересечения (если существует)
 * @return bool true, если линии пересекаются, иначе false
 */
bool Line::Intersection( const Line& rLine, Point& rIntersection ) const
{
    double  fX, fY;
    bool    bRet;

    if( Intersection( rLine, fX, fY ) )
    {
        rIntersection.setX(basegfx::fround<tools::Long>(fX));
        rIntersection.setY(basegfx::fround<tools::Long>(fY));
        bRet = true;
    }
    else
        bRet = false;

    return bRet;
}

/**
 * @brief Поиск точки пересечения двух линий с точностью до вещественных чисел.
 *
 * Определяет, пересекаются ли две линии, и если да, то вычисляет
 * координаты точки пересечения в вещественных числах.
 *
 * @param rLine Вторая линия
 * @param[out] rIntersectionX X-координата точки пересечения
 * @param[out] rIntersectionY Y-координата точки пересечения
 * @return bool true, если линии пересекаются, иначе false
 */
bool Line::Intersection( const tools::Line& rLine, double& rIntersectionX, double& rIntersectionY ) const
{
    const double    fAx = static_cast<double>(maEnd.X()) - maStart.X();
    const double    fAy = static_cast<double>(maEnd.Y()) - maStart.Y();
    const double    fBx = static_cast<double>(rLine.maStart.X()) - rLine.maEnd.X();
    const double    fBy = static_cast<double>(rLine.maStart.Y()) - rLine.maEnd.Y();
    const double    fDen = fAy * fBx - fAx * fBy;
    bool            bOk = false;

    if( fDen != 0. )
    {
        const double    fCx = static_cast<double>(maStart.X()) - rLine.maStart.X();
        const double    fCy = static_cast<double>(maStart.Y()) - rLine.maStart.Y();
        const double    fA = fBy * fCx - fBx * fCy;
        const bool      bGreater = ( fDen > 0. );

        bOk = true;

        if ( bGreater )
        {
            if ( ( fA < 0. ) || ( fA > fDen ) )
                bOk = false;
        }
        else if ( ( fA > 0. ) || ( fA < fDen ) )
            bOk = false;

        if ( bOk )
        {
            const double fB = fAx * fCy - fAy * fCx;

            if ( bGreater )
            {
                if ( ( fB < 0. ) || ( fB > fDen ) )
                    bOk = false;
            }
            else if ( ( fB > 0. ) || ( fB < fDen ) )
                bOk = false;

            if( bOk )
            {
                const double fAlpha = fA / fDen;

                rIntersectionX = ( maStart.X() + fAlpha * fAx );
                rIntersectionY = ( maStart.Y() + fAlpha * fAy );
            }
        }
    }

    return bOk;
}

/**
 * @brief Вычисление расстояния от точки до линии.
 *
 * Рассчитывает кратчайшее расстояние от заданной точки до линии.
 * Если точка проецируется за пределы линии, расстояние вычисляется
 * до ближайшего конца линии.
 *
 * @param rPtX X-координата точки
 * @param rPtY Y-координата точки
 * @return double Расстояние от точки до линии
 */
double Line::GetDistance( const double& rPtX, const double& rPtY ) const
{
    double fDist;

    if( maStart != maEnd )
    {
        const double fDistX = static_cast<double>(maEnd.X()) - maStart.X();
        const double fDistY = static_cast<double>(maEnd.Y()) - maStart.Y();
        const double fACX = static_cast<double>(maStart.X()) - rPtX;
        const double fACY = static_cast<double>(maStart.Y()) - rPtY;
        const double fL2 = fDistX * fDistX + fDistY * fDistY;
        const double fR = ( fACY * -fDistY - fACX * fDistX ) / fL2;
        const double fS = ( fACY * fDistX - fACX * fDistY ) / fL2;

        if( fR < 0.0 )
        {
            fDist = hypot( maStart.X() - rPtX, maStart.Y() - rPtY );

            if( fS < 0.0 )
                fDist *= -1.0;
        }
        else if( fR <= 1.0 )
            fDist = fS * sqrt( fL2 );
        else
        {
            fDist = hypot( maEnd.X() - rPtX, maEnd.Y() - rPtY );

            if( fS < 0.0 )
                fDist *= -1.0;
        }
    }
    else
        fDist = hypot( maStart.X() - rPtX, maStart.Y() - rPtY );

    return fDist;
}

} //namespace tools

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
