# One element test to test the central difference time integrator in 3D.
[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
  volumetric_locking_correction = true
  #use_legacy_initial_residual_evaluation_behavior = true
[]

[Mesh]
  [block_one]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 3
    ny = 3
    nz = 3
    xmin = 4.5
    xmax = 5.5
    ymin = 4.5
    ymax = 5.5
    zmin = 0.5
    zmax = 1.5
    boundary_name_prefix = 'ball'
  []
  [block_two]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 2
    ny = 2
    nz = 2
    xmin = 0.0
    xmax = 10
    ymin = 0.0
    ymax = 10
    zmin = -2
    zmax = 0
    boundary_name_prefix = 'base'
    boundary_id_offset = 10
  []
  [block_one_id]
    type = SubdomainIDGenerator
    input = block_one
    subdomain_id = 1
  []
  [block_two_id]
    type = SubdomainIDGenerator
    input = block_two
    subdomain_id = 2
  []
  [combine]
    type = MeshCollectionGenerator
    inputs = ' block_one_id block_two_id'
  []
[]

[AuxVariables]
  [penetration]
  []
[]

[AuxKernels]
  [penetration]
    type = PenetrationAux
    variable = penetration
    boundary = ball_back
    paired_boundary = base_front
    quantity = distance
  []
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
  [gap_rate]
  []
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
  [stress_zz]
    family = MONOMIAL
    order = CONSTANT
  []
  [strain_zz]
    family = MONOMIAL
    order = CONSTANT
  []
  [kinetic_energy_one]
    order = CONSTANT
    family = MONOMIAL
  []
  [elastic_energy_one]
    order = CONSTANT
    family = MONOMIAL
  []
  [kinetic_energy_two]
    order = CONSTANT
    family = MONOMIAL
  []
  [elastic_energy_two]
    order = CONSTANT
    family = MONOMIAL
  []
  [potential_energy]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [accel_x]
    type = NewmarkAccelAux
    variable = accel_x
    displacement = disp_x
    velocity = vel_x
    beta = 0.25
    execute_on = 'LINEAR timestep_end'
  []
  [vel_x]
    type = NewmarkVelAux
    variable = vel_x
    acceleration = accel_x
    gamma = 0.5
    execute_on = 'LINEAR timestep_end'
  []
  [accel_y]
    type = NewmarkAccelAux
    variable = accel_y
    displacement = disp_y
    velocity = vel_y
    beta = 0.25
    execute_on = 'LINEAR timestep_end'
  []
  [vel_y]
    type = NewmarkVelAux
    variable = vel_y
    acceleration = accel_y
    gamma = 0.5
    execute_on = 'LINEAR timestep_end'
  []
[]

[stress_zz]
  type = RankTwoAux
  rank_two_tensor = stress
  index_i = 2
  index_j = 2
  variable = stress_zz
  execute_on = 'TIMESTEP_END'
[]
[strain_zz]
  type = RankTwoAux
  rank_two_tensor = mechanical_strain
  index_i = 2
  index_j = 2
  variable = strain_zz
[]

#----------------------------------------------------------
[kinetic_energy_one]
  type = KineticEnergyAux
  block = '1'
  variable = kinetic_energy_one
  newmark_velocity_x = vel_x
  newmark_velocity_y = vel_y
  newmark_velocity_z = vel_z
  density = density
[]
[elastic_energy_one]
  type = ElasticEnergyAux
  variable = elastic_energy_one
  block = '1'
[]
[kinetic_energy_two]
  type = KineticEnergyAux
  block = '2'
  variable = kinetic_energy_two
  newmark_velocity_x = vel_x
  newmark_velocity_y = vel_y
  newmark_velocity_z = vel_z
  density = density
[]
[elastic_energy_two]
  type = ElasticEnergyAux
  variable = elastic_energy_two
  block = '2'
[]
[potential_energy]
  type = FunctionAux
  variable = potential_energy
  function = poteng
  use_displaced_mesh = true
  block = 1
[]
[Debug]
  show_var_residual_norms = true
[]

[Outputs]
  exodus = true
  checkpoint = true
[]

[Preconditioning]
  [smp]
    type = SMP
    full = true
  []
[]

[Postprocessors]
  active = ''
  [num_nl]
    type = NumNonlinearIterations
  []
  [cumulative]
    type = CumulativeValuePostprocessor
    postprocessor = num_nl
  []
[]
