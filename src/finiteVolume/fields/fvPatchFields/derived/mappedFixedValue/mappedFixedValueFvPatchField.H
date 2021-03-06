/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::mappedFixedValueFvPatchField

Description
    Recycles the value at a set of cells or patch faces back to *this. Can not
    sample internal faces (since volField not defined on faces).

    mode = NEARESTCELL : sample nearest cell
    mode = NEARESTPATCHFACE : sample nearest face on selected patch
    mode = NEARESTFACE : sample nearest face on any patch. Note: does not
           warn if nearest actually is on internal face!
    mode = NEARESTPATCHFACEAMI : sample nearest face on selected patch
           patches can be non-conformal - method uses AMI interpolation

    For NEARESTCELL you have to provide an 'interpolationScheme' entry
    which can be any one of the interpolation schemes (cell, cellPoint, etc.)
    In case of interpolation (so scheme != cell) the limitation is that
    there is only one value per cell. So e.g. if you have a cell with two
    boundary faces and both faces sample into the cell both faces will get
    the same value.

    See mappedPatchBase for options on sampling.

    Optional 'fieldName' entry to supply a different filename

SourceFiles
    mappedFixedValueFvPatchField.C

\*---------------------------------------------------------------------------*/

#ifndef mappedFixedValueFvPatchField_H
#define mappedFixedValueFvPatchField_H

#include "fixedValueFvPatchFields.H"
#include "interpolation.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                  Class mappedFixedValueFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class mappedFixedValueFvPatchField
:
    public fixedValueFvPatchField<Type>
{

protected:

    // Protected data

        //- Name of field to sample - defaults to field associated with this
        //  patchField if not specified
        word fieldName_;

        //- If true adjust the mapped field to maintain average value average_
        const bool setAverage_;

        //- Average value the mapped field is adjusted to maintain if
        //  setAverage_ is set true
        const Type average_;

        //- Interpolation scheme to use for nearestcell mode
        word interpolationScheme_;

        mutable autoPtr<interpolation<Type> > interpolator_;


    // Protected Member Functions

        //- Field to sample. Either on my or nbr mesh
        const GeometricField<Type, fvPatchField, volMesh>& sampleField() const;

        //- Access the interpolation method
        const interpolation<Type>& interpolator() const;


public:

    //- Runtime type information
    TypeName("mapped");


    // Constructors

        //- Construct from patch and internal field
        mappedFixedValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        mappedFixedValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given mappedFixedValueFvPatchField
        //  onto a new patch
        mappedFixedValueFvPatchField
        (
            const mappedFixedValueFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        mappedFixedValueFvPatchField
        (
            const mappedFixedValueFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new mappedFixedValueFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        mappedFixedValueFvPatchField
        (
            const mappedFixedValueFvPatchField<Type>&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchField<Type> > clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return tmp<fvPatchField<Type> >
            (
                new mappedFixedValueFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "mappedFixedValueFvPatchField.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
