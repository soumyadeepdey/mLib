
namespace ml {

TriMesh meshutil::createUnifiedMesh(const std::vector< std::pair<TriMesh, mat4f> >& meshes) {
  auto lambdaVertices = [ = ](size_t total, const std::pair<TriMesh, mat4f>& t) { return t.first.vertices().size() + total; };
  const size_t totalPoints = std::accumulate(meshes.begin(), meshes.end(), static_cast<size_t>(0), lambdaVertices);
  auto lambdaIndices = [ = ](size_t total, const std::pair<TriMesh, mat4f>& t) { return t.first.indices().size() + total; };
  const size_t totalIndices = std::accumulate(meshes.begin(), meshes.end(), static_cast<size_t>(0), lambdaIndices);

  std::vector<TriMesh::TriMeshVertex> vertices(totalPoints);
  std::vector<UINT> indices(totalIndices);

  UINT vIndex = 0, iIndex = 0;
  for (const auto& m : meshes) {
    const UINT baseVertexIndex = vIndex;

    for (UINT vertexIndex = 0; vertexIndex < m.first.vertices().size(); vertexIndex++) {
      TriMesh::TriMeshVertex& v = vertices[vIndex++];
      v = m.first.vertices()[vertexIndex];
      v.position =  m.second * v.position;
    }
    for (UINT indexIndex = 0; indexIndex < m.first.indices().size(); indexIndex++) {
      indices[iIndex++] = m.first.indices()[indexIndex] + baseVertexIndex;
    }
  }

  return TriMesh(vertices, indices);
}

TriMesh meshutil::createUnifiedMesh(const std::vector<TriMesh>& meshes) {
  auto lambdaVertices = [=](size_t total, const TriMesh& t) { return t.vertices().size() + total; };
  const size_t totalPoints = std::accumulate(meshes.begin(), meshes.end(), static_cast<size_t>(0), lambdaVertices);
  auto lambdaIndices = [=](size_t total, const TriMesh& t) { return t.indices().size() + total; };
  const size_t totalIndices = std::accumulate(meshes.begin(), meshes.end(), static_cast<size_t>(0), lambdaIndices);

  std::vector<TriMesh::TriMeshVertex> vertices(totalPoints);
  std::vector<UINT> indices(totalIndices);

  UINT vIndex = 0, iIndex = 0;
  for (const auto& m : meshes) {
    const UINT baseVertexIndex = vIndex;

    for (UINT vertexIndex = 0; vertexIndex < m.vertices().size(); vertexIndex++) {
      TriMesh::TriMeshVertex& v = vertices[vIndex++];
      v = m.vertices()[vertexIndex];
    }
    for (UINT indexIndex = 0; indexIndex < m.indices().size(); indexIndex++) {
      indices[iIndex++] = m.indices()[indexIndex] + baseVertexIndex;
    }
  }

  return TriMesh(vertices, indices);
}

TriMesh meshutil::createPointCloudTemplate(const TriMesh& templateMesh,
                                           const std::vector<vec3f>& points,
                                           const std::vector<vec4f>& colors) {
  const UINT64 pointCount = points.size();
  const UINT64 tVertices = templateMesh.vertices().size();
  const UINT64 tIndices = templateMesh.indices().size();
  const vec4f defaultColor(1.f, 0.f, 0.f, 1.f);

  std::vector<TriMesh::TriMeshVertex> vertices(pointCount * tVertices);
  std::vector<UINT> indices(pointCount * tIndices);

  for (UINT pointIndex = 0; pointIndex < points.size(); pointIndex++) {
    const vec3f& p = points[pointIndex];
    const vec4f& c = colors.empty() ? defaultColor : colors[pointIndex];
    const UINT64 baseVertexIndex = pointIndex * tVertices;

    for (UINT vertexIndex = 0; vertexIndex < tVertices; vertexIndex++) {
      TriMesh::TriMeshVertex& v = vertices[baseVertexIndex + vertexIndex];
      v = templateMesh.vertices()[vertexIndex];
      v.position += p;
      v.attributeA = c;
    }
    for (UINT indexIndex = 0; indexIndex < tIndices; indexIndex++) {
      indices[pointIndex * tIndices + indexIndex] = templateMesh.indices()[indexIndex] + (pointIndex * (UINT)tVertices);
    }
  }

  return TriMesh(vertices, indices);
}

}  // namespace ml
