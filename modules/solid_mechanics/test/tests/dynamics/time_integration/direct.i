# Test for  Direct explict time integration

# The test is for an 1D bar element of  unit length fixed on one end
# with a ramped pressure boundary condition applied to the other end.
# beta and gamma are Newmark time integration parameters
# The equation of motion in terms of matrices is:
#
# M*accel + K*disp = P*Area
#
# Here M is the mass matrix, K is the stiffness matrix, P is the applied pressure
#
# This equation is equivalent to:
#
# density*accel + Div Stress = P
#
# The first term on the left is evaluated using the Inertial force kernel
# The last term on the left is evaluated using StressDivergenceTensors
# The residual due to Pressure is evaluated using Pressure boundary condition

[Mesh]
    type = GeneratedMesh
    dim = 3
    xmax = 0.1
    ymax = 1.0
    zmax = 0.1
[]

[Variables]
    [disp_x]
    []
    [disp_y]
    []
    [disp_z]
    []
[]

[AuxVariables]
    [vel_x]
    []
    [accel_x]
    []
    [vel_y]
    []
    [accel_y]
    []
    [vel_z]
    []
    [accel_z]
    []
    [stress_yy]
        order = CONSTANT
        family = MONOMIAL
    []
    [strain_yy]
        order = CONSTANT
        family = MONOMIAL
    []
[]

[Kernels]
    [SolidMechanics]
        displacements = 'disp_x disp_y disp_z'
    []
    [inertia_x]
        type = InertialForce
        variable = disp_x
    []
    [inertia_y]
        type = InertialForce
        variable = disp_y
    []
    [inertia_z]
        type = InertialForce
        variable = disp_z
    []
[]

[AuxKernels]
    [stress_yy]
        type = RankTwoAux
        rank_two_tensor = stress
        variable = stress_yy
        index_i = 1
        index_j = 1
    []
    [strain_yy]
        type = RankTwoAux
        rank_two_tensor = total_strain
        variable = strain_yy
        index_i = 1
        index_j = 1
    []
[]

[BCs]
    [top_x]
        type = DirichletBC
        variable = disp_x
        boundary = top
        value = 0.0
        preset = false
    []
    [top_y]
        type = DirichletBC
        variable = disp_y
        boundary = top
        value = 0.0
        preset = false
    []
    [top_z]
        type = DirichletBC
        variable = disp_z
        boundary = top
        value = 0.0
        preset = false
    []
    [Pressure]
        [Side1]
            boundary = bottom
            function = pressure
            factor = 1
            displacements = 'disp_x disp_y disp_z'
        []
    []
[]

[Materials]
    [Elasticity_tensor]
        type = ComputeElasticityTensor
        fill_method = symmetric_isotropic
        C_ijkl = '210 0'
    []

    [strain]
        type = ComputeSmallStrain
        displacements = 'disp_x disp_y disp_z'
    []

    [stress]
        type = ComputeLinearElasticStress
    []
    [density]
        type = GenericConstantMaterial
        prop_names = 'density'
        prop_values = '7750'
    []
[]

[Executioner]
    type = Transient
    start_time = 0
    end_time = 2
    dt = 0.01
    [TimeIntegrator]
        type = CentralDifference
        solve_type = LUMPED
        use_direct = true
    []
[]

[Functions]
    [pressure]
        type = PiecewiseLinear
        x = '0.0 0.2 1.0 5.0'
        y = '0.0 0.2 1.0 1.0'
        scale_factor = 1e3
    []
[]

[Postprocessors]
    [dt]
        type = TimestepSize
    []
    [disp]
        type = NodalExtremeValue
        variable = disp_y
        boundary = bottom
    []
    [vel]
        type = NodalExtremeValue
        variable = vel_y
        boundary = bottom
    []
    [accel]
        type = NodalExtremeValue
        variable = accel_y
        boundary = bottom
    []
    [stress_yy]
        type = ElementAverageValue
        variable = stress_yy
    []
    [strain_yy]
        type = ElementAverageValue
        variable = strain_yy
    []
[]

[Outputs]
    interval = 10
    exodus = true
    perf_graph = true
[]
